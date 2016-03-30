#include <iostream>
#include "OGRE\Ogre.h"
#include "Ogre.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreEntity.h"
#include "MyFrameListener.cpp"

using namespace std;

class MyApplication {
	private: 
		Ogre::SceneManager* _sceneManager;
		Ogre::Root* _root;
		MyFrameListener* _listener;
		Ogre::RaySceneQuery* _ray_scene_query;
		bool _keepRunning;
	public: 
		MyApplication() {
			_sceneManager = NULL;
			_root = NULL;
			_listener = NULL;
		}

		~MyApplication() {
			delete _root;
			delete _listener;
		}

		void loadResources() {
			Ogre::ConfigFile cf;
			cf.load("../LabFiles/OgreConfig/resources_d.cfg");
			Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
			Ogre::String sectionName, typeName, dataname;
			while(sectionIter.hasMoreElements()) {
				sectionName = sectionIter.peekNextKey();
				Ogre::ConfigFile::SettingsMultiMap* settings = sectionIter.getNext();
				Ogre::ConfigFile::SettingsMultiMap::iterator i;
				for(i = settings->begin(); i != settings->end(); i++) {
					typeName = i->first;
					dataname = i->second;
					Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
				}
			}
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}
		int startup() {
			_root = new Ogre::Root("../LabFiles/OgreConfig/plugins_d.cfg");
			if(!_root->showConfigDialog()) {
				return -1;
			}

			Ogre::RenderWindow* window = _root -> initialise(true, "Ogre3D Beginners Guide");
			_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

			// create the ray scene query object
			_ray_scene_query = _sceneManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
			if (NULL == _ray_scene_query)
			{
				cout << "Failed to create Ogre::RaySceneQuery instance" << endl;
				return (false);
			}
			_ray_scene_query->setSortByDistance(true);
			
			Ogre::Camera* camera = _sceneManager->createCamera("Camera");
			camera->setPosition(Ogre::Vector3(0, 50, 25));
			camera->lookAt(Ogre::Vector3(0, 0, 0));
			camera->setNearClipDistance(5);

			Ogre::Viewport* viewport = window->addViewport(camera);
			viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
			camera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));
			
			_listener = new MyFrameListener(window, camera);
			_root->addFrameListener(_listener);

			loadResources();
			createScene();
			_root->startRendering();
			return 0;
		}

		void renderOneFrame() {
			Ogre::WindowEventUtilities::messagePump();
			_keepRunning = _root->renderOneFrame();
		}

		bool keepRunning() {
			return _keepRunning;
		}

		void createScene() 
		{
			Ogre::Entity* world = _sceneManager->createEntity("world", "Plane.mesh");
			cout << "created world entity \n";
			Ogre::SceneNode* modelNode = _sceneManager->getRootSceneNode();//->createChildSceneNode();
			
			cout << "created model scene node\n";
			
			modelNode->setPosition(0,0,0);
			//modelNode->setScale(100,1,100); // You may have to scale your object to see it well
			modelNode->attachObject(world);
			cout << "attached world model to scene node\n";
			/*
			Ogre::AxisAlignedBox worldbox = _sceneManager->getEntity("world")->getBoundingBox();
			_sceneManager->createEntity("marker1", Ogre::SceneManager::PT_SPHERE);
			Ogre::SceneNode* markerNode1 = _sceneManager->getRootSceneNode()->createChildSceneNode();
			markerNode1->attachObject(_sceneManager->getEntity("marker1"));
			Ogre::Vector3 farRightCorner = worldbox.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_TOP);
			markerNode1->setPosition(farRightCorner);
			*/

			Ogre::AxisAlignedBox worldbox = _sceneManager->getEntity("world")->getBoundingBox();
			Ogre::Vector3 farRightCorner = worldbox.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_TOP);
			Ogre::Vector3 farLeftCorner = worldbox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_TOP);
			Ogre::Vector3 nearRightCorner = worldbox.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_TOP);
			cout << "far right corner: " << farRightCorner << endl;
			cout << "far left corner: " << farLeftCorner << endl;
			cout << "near right corner: " << nearRightCorner << endl;

			Ogre::Entity* sinbad = _sceneManager->createEntity("sinbad", "Sinbad.mesh");
			Ogre::SceneNode* markerNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
			markerNode->attachObject(sinbad);
			markerNode->setPosition(modelNode->getPosition());
			
			Ogre::Entity* marker2 = _sceneManager->createEntity("sinbad2", "Sinbad.mesh");
			Ogre::SceneNode* markerNode2 = _sceneManager->getRootSceneNode()->createChildSceneNode();
			markerNode2->attachObject(_sceneManager->getEntity("sinbad2"));
			markerNode2->setPosition(farRightCorner);

			/* 
			hokay so over x and z double loop
			*/

			Ogre::Real resolution = 1;

			for(Ogre::Real i = farRightCorner.x; i > farLeftCorner.x; i -= resolution) {
				for(Ogre::Real j = farRightCorner.z; j < nearRightCorner.z; j+= resolution) {
					/*cout << "Placing marker at " << i << ", " << j << endl;
					Ogre::Entity* ent = _sceneManager->createEntity("Sinbad.mesh");
					Ogre::SceneNode* testNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
					testNode->attachObject(ent);
					testNode->setPosition(i, 0, j);*/
				}
			}


			//ugly but working world/height map generation 
			Ogre::Entity* _myCubes[10][10];
		std::pair<Ogre::Vector3,Ogre::Real> _myCubesWaypoints[10][10];
		bool _myCubesBool[10][10] = 
		{
			{1,0,1,1,1,1,1,1,1,1},
			{1,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,1},
			{1,0,1,0,1,0,1,1,1,1},
			{1,0,0,0,0,0,1,0,0,1},
			{1,1,1,0,0,1,1,1,0,1},
			{1,0,0,1,0,1,0,0,0,1},
			{1,0,1,1,0,1,0,1,0,1},
			{1,0,0,0,0,0,0,1,0,1},
			{1,1,1,1,1,1,1,1,0,1}
			
		};
		Ogre::Real posX = 120;
		Ogre::Real posZ = 0;
		for(int x = 0; x < 10 ; x++)
		{
			int my_intX = x+1;
			for(int i = 0; i < 10 ; i++)
			{
				Ogre::String name = "CubeX ";
				
				int my_intY = i+1;
				name.append(Ogre::StringConverter::toString(my_intX));
				name.append(" Y ");
				name.append(Ogre::StringConverter::toString(my_intY));
				_myCubesWaypoints[x][i] = std::make_pair(Ogre::Vector3(posX,0,posZ),0);
				if(_myCubesBool[x][i])
				{
					_myCubes[x][i] =  _sceneManager->createEntity(name, "Cube.mesh");
					//_myCubes[x][i]->setMaterialName("shader/orange");
					//const Ogre::AxisAlignedBox& CubeBoundingBox=_myCubes[x][i]->getWorldBoundingBox(true);
						

					Ogre::SceneNode* cubenodeX = _sceneManager->getRootSceneNode()->createChildSceneNode();
				//cubenode2->attachObject(_myCube2);
					cubenodeX->scale(0.1, 0.1, 0.1);    //Try different values
					cubenodeX->setPosition(posX, 0.0, posZ);
					cubenodeX->showBoundingBox(true);
					cubenodeX->attachObject(_myCubes[x][i]);

					Ogre::Vector3 RayOrigin(posX,100,posZ);
					Ogre::Vector3 RayDir(0,-10,0);
					Ogre::Ray CubeRay(RayOrigin,RayDir);
					std::pair<bool, Ogre::Real> point2 = CubeRay.intersects(_myCubes[x][i]->getWorldBoundingBox(true));
					if (point2.first)
					{
						Ogre::Vector3 height = CubeRay.getPoint(point2.second);
						//std::cout << "the height at " << name << " is " << height.y << std::endl;
						_myCubesWaypoints[x][i].first.y = height.y;
						_myCubesWaypoints[x][i].second = Ogre::Math::Floor(height.y);
					}
					
					
				}
				posX += 10;
				
			}
			posX = 120;
			posZ += 10;
		}
		
		//print out height map
		std::cout << "printing out height map " << std::endl;
		for(int x = 0; x < 10 ; x++)
		{
			std::cout << "{ ";
			for(int i = 0; i < 10 ; i++)
			{
				std::cout << _myCubesWaypoints[x][i].second << " , ";
			}
			std::cout << "}" << std::endl;
		}
 
		//
		
		//
	Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material","General"); 
myManualObjectMaterial->setReceiveShadows(false); 
myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 

	Ogre::MaterialPtr myManualObjectMaterial2 = Ogre::MaterialManager::getSingleton().create("manual2Material","General"); 
myManualObjectMaterial2->setReceiveShadows(false); 
myManualObjectMaterial2->getTechnique(0)->setLightingEnabled(true); 
myManualObjectMaterial2->getTechnique(0)->getPass(0)->setDiffuse(1,0,0,0); 
myManualObjectMaterial2->getTechnique(0)->getPass(0)->setAmbient(1,0,0); 
myManualObjectMaterial2->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0); 

	for (int x = 0; x < 10; x++)
	{
		int my_intX = x+1;
		for (int i = 0; i < 10; i++)
		{
			Ogre::String name = "GridLine X";
			Ogre::String name2 = "GridLine2 X";
				int my_intY = i+1;
				name.append(Ogre::StringConverter::toString(my_intX));
				name.append(" Y ");
				name.append(Ogre::StringConverter::toString(my_intY));
				name2.append(Ogre::StringConverter::toString(my_intX));
				name2.append(" Y ");
				name2.append(Ogre::StringConverter::toString(my_intY));
				Ogre::ManualObject *manual = static_cast<Ogre::ManualObject*>(_sceneManager->createMovableObject(name, Ogre::ManualObjectFactory::FACTORY_TYPE_NAME));
				Ogre::ManualObject *manual2 = static_cast<Ogre::ManualObject*>(_sceneManager->createMovableObject(name2, Ogre::ManualObjectFactory::FACTORY_TYPE_NAME));
				Ogre::SceneNode* gridNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
			if(x == 9)
			{
				if(i == 9)
				{

				}
				else
				{
					
					Ogre::Vector3 start = _myCubesWaypoints[x][i].first;
					Ogre::Vector3 endX = _myCubesWaypoints[x][i+1].first;
					if(Ogre::Math::Abs((start.y*start.y)-(endX.y*endX.y)) > 20)
					{
						manual->begin("manual2Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					else
					{
						manual->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					//draw line from point [x][y] to [x][y+1]
					manual->position(start.x, start.y+5, start.z);
					manual->position(endX.x, endX.y+5, endX.z);
      
					manual->end();
					gridNode->attachObject(manual);
				}
			}
			else
			{
				Ogre::Vector3 start = _myCubesWaypoints[x][i].first;
					Ogre::Vector3 endX = _myCubesWaypoints[x][i+1].first;
					Ogre::Vector3 endY = _myCubesWaypoints[x+1][i].first;
				if(i == 9)
				{
				}
				else
				{
					if(Ogre::Math::Abs((start.y*start.y)-(endX.y*endX.y)) > 20)
					{
						manual->begin("manual2Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					else
					{
						manual->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					//draw line from point [x][y] to [x][y+1]
					manual->position(start.x, start.y+5, start.z);
					manual->position(endX.x, endX.y+5, endX.z);
      
					manual->end();
					gridNode->attachObject(manual);

					if(Ogre::Math::Abs((start.y*start.y)-(endY.y*endY.y)) > 20)
					{
						manual2->begin("manual2Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					else
					{
						manual2->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST);
					}
					//draw line from point [x][y] to [x+1][y]
					manual2->position(start.x, start.y+5, start.z);
					manual2->position(endY.x, endY.y+5, endY.z);
      
					manual2->end();
					gridNode->attachObject(manual2);
				}
			}
		}
	}
			//

			
			Ogre::Vector3 raycastresult;

			if(RaycastFromPoint(Ogre::Vector3(0, 5, 0), Ogre::Vector3(0, -1, 0), raycastresult)) {
				cout << "Ray hit at " << raycastresult << endl;
			} else {
				cout << "Ray miss" << endl;
			}

			Ogre::Ray ray(Ogre::Vector3(0, 5, 0), Ogre::Vector3(0, -1, 0));

			std::pair<bool, Ogre::Real> manualrayresult = ray.intersects(_sceneManager->getEntity("sinbad")->getBoundingBox());

			if(manualrayresult.first) {
				cout << "Manual ray hit at " << ray.getPoint(manualrayresult.second) << endl;
			} else {
				cout << "Manual ray miss" << endl;
			}

			_sceneManager->setAmbientLight(Ogre::ColourValue(0.6f,0.6f,0.6f));
			Ogre::Light* light = _sceneManager->createLight("Light1");
			light->setType(Ogre::Light::LT_DIRECTIONAL);
			light->setDirection(Ogre::Vector3(1, -1, 0));

			_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		}

		// raycast from a point in to the scene.
		// returns success or failure.
		// on success the point is returned in the result.
		bool RaycastFromPoint(const Ogre::Vector3 &point,
												const Ogre::Vector3 &normal,
												Ogre::Vector3 &result)
		{
			// create the ray to test
			Ogre::Ray ray(Ogre::Vector3(point.x, point.y, point.z),
						  Ogre::Vector3(normal.x, normal.y, normal.z));

			cout << "Ray origin: " << ray.getOrigin() << " Ray direction: " << ray.getDirection() << endl;
 
			// check we are initialised
			if (_ray_scene_query != NULL)
			{
				// create a query object
				_ray_scene_query->setRay(ray);
 
				// execute the query, returns a vector of hits
				if (_ray_scene_query->execute().size() <= 0)
				{
					// raycast did not hit an objects bounding box
					return (false);
				}
			}
			else
			{
				cout << "Cannot raycast without RaySceneQuery instance" << endl;
				return (false);
			}   
 
			// at this point we have raycast to a series of different objects bounding boxes.
			// we need to test these different objects to see which is the first polygon hit.
			// there are some minor optimizations (distance based) that mean we wont have to
			// check all of the objects most of the time, but the worst case scenario is that
			// we need to test every triangle of every object.
			Ogre::Real closest_distance = -1.0f;
			Ogre::Vector3 closest_result;
			Ogre::RaySceneQueryResult &query_result = _ray_scene_query->getLastResults();
			for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
			{
				// stop checking if we have found a raycast hit that is closer
				// than all remaining entities
				if ((closest_distance >= 0.0f) &&
					(closest_distance < query_result[qr_idx].distance))
				{
					 break;
				}
 
				// only check this result if its a hit against an entity
				if ((query_result[qr_idx].movable != NULL) &&
					(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
				{
					// get the entity to check
					Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           
 
					// mesh data to retrieve         
					size_t vertex_count;
					size_t index_count;
					Ogre::Vector3 *vertices;
					unsigned long *indices;
 
					// get the mesh information
				 GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
									  pentity->getParentNode()->_getDerivedPosition(),
									  pentity->getParentNode()->_getDerivedOrientation(),
									  pentity->getParentNode()->_getDerivedScale());
 
					// test for hitting individual triangles on the mesh
					bool new_closest_found = false;
					for (int i = 0; i < static_cast<int>(index_count); i += 3)
					{
						// check for a hit against this triangle
						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
							vertices[indices[i+1]], vertices[indices[i+2]], true, false);
 
						// if it was a hit check if its the closest
						if (hit.first)
						{
							if ((closest_distance < 0.0f) ||
								(hit.second < closest_distance))
							{
								// this is the closest so far, save it off
								closest_distance = hit.second;
								new_closest_found = true;
							}
						}
					}
 
				 // free the verticies and indicies memory
					delete[] vertices;
					delete[] indices;
 
					// if we found a new closest raycast for this object, update the
					// closest_result before moving on to the next object.
					if (new_closest_found)
					{
						closest_result = ray.getPoint(closest_distance);               
					}
				}       
			}
 
			// return the result
			if (closest_distance >= 0.0f)
			{
				// raycast success
				result = closest_result;
				return (true);
			}
			else
			{
				// raycast failed
				return (false);
			} 
		}

		// Get the mesh information for the given mesh.
		// Code found on this forum link: http://www.ogre3d.org/wiki/index.php/RetrieveVertexData
		void GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}
};