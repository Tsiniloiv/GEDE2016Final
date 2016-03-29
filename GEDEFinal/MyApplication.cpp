#include <iostream>
#include "OGRE\Ogre.h"
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

		void createScene() {
			Ogre::Entity* world = _sceneManager->createEntity("world", "Plane.mesh");
			cout << "created world entity \n";
			Ogre::SceneNode* modelNode = _sceneManager->getRootSceneNode();//->createChildSceneNode();
			cout << "created model scene node\n";
			modelNode->attachObject(world);
			cout << "attached world model to scene node\n";
			modelNode->setPosition(0,0,0);
			//modelNode->setScale(100,0,100); // You may have to scale your object to see it well\\
			
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
					cout << "Placing marker at " << i << ", " << j << endl;
					Ogre::Entity* ent = _sceneManager->createEntity("Sinbad.mesh");
					Ogre::SceneNode* testNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
					testNode->attachObject(ent);
					testNode->setPosition(i, 0, j);
				}
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
				 OgreVE::GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
									  pentity->getParentNode()->getWorldPosition(),
									  pentity->getParentNode()->getWorldOrientation(),
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
		};