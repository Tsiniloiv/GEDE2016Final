#include <iostream>
#include "OGRE/Ogre.h"
#include "Ogre.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreMesh.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreEntity.h"
#include "MyFrameListener.cpp"

using namespace std;
#define gridSize 20
class MyApplication {
	private: 
		Ogre::SceneManager* _sceneManager;
		Ogre::Root* _root;
		MyFrameListener* _listener;
		Ogre::RaySceneQuery* _ray_scene_query;
		bool _keepRunning;
		Ogre::SceneNode* markerNode;

		Ogre::Entity* _myCubes[gridSize][gridSize];
		std::pair<Ogre::Vector3,Ogre::Real> _myCubesWaypoints[gridSize][gridSize];
		int size;
		//move on path
		std::deque<Ogre::Vector3> mWalkList;
		Ogre::Vector3 mDirection;
		Ogre::Vector3 mDestination;

	public: 
		MyApplication() {
			_sceneManager = NULL;
			_root = NULL;
			_listener = NULL;
			size = gridSize;

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
			
		

			loadResources();
			createScene();
			_listener = new MyFrameListener(window, camera,mWalkList,markerNode);
			_root->addFrameListener(_listener);
			_root->startRendering();
			return 0;
		}

		void renderHeightmap()
		{

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

	for (int x = 0; x < size; x++)
	{
		int my_intX = x+1;
		for (int i = 0; i < size; i++)
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
			if(x == size-1)
			{
				if(i == size-1)
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
				if(i == size-1)
				{
				}
				else
				{
					if(Ogre::Math::Abs(Ogre::Math::Sqrt(start.y*start.y)-Ogre::Math::Sqrt(endX.y*endX.y)) > 6)
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

					if(Ogre::Math::Abs((Ogre::Math::Sqrt(start.y*start.y))-(Ogre::Math::Sqrt(endY.y*endY.y))) > 6)
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

		}

		void renderMaze()
		{
		const int size = gridSize;
		//basic maze blueprint
		int _myCubesBool[size][size] = 
		{
			{2,0,2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2},
			{2,0,2,0,0,0,0,0,0,2,4,0,2,0,0,0,0,0,0,2},
			{2,0,0,0,2,2,2,2,0,2,2,0,0,0,2,2,2,2,0,2},
			{2,0,2,0,2,0,2,2,2,2,2,0,2,0,2,0,2,2,2,2},
			{2,0,0,0,0,0,2,0,0,2,2,0,0,0,0,0,2,0,0,2},
			{2,2,2,0,0,3,3,3,0,2,2,2,2,0,0,3,2,2,0,2},
			{2,0,0,2,0,3,1,0,0,2,2,0,0,2,0,2,0,0,0,2},
			{2,0,2,2,0,3,0,3,0,2,2,0,2,2,0,2,0,2,0,2},
			{2,0,0,0,0,0,0,2,0,2,2,0,0,0,0,0,0,2,0,2},
			{2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,0,2},
			{2,0,2,2,2,2,2,2,0,2,2,0,2,2,2,2,2,2,0,2},
			{2,0,2,0,0,0,0,0,0,2,2,0,2,0,0,0,0,0,0,2},
			{2,0,0,0,2,2,2,2,0,2,2,0,0,0,2,2,2,2,0,2},
			{2,0,2,0,2,0,2,2,2,2,2,0,2,0,2,0,2,2,2,2},
			{2,0,0,0,0,0,2,0,0,2,2,0,0,0,0,0,2,0,0,2},
			{2,2,2,0,0,2,2,2,0,2,2,2,2,0,0,2,2,2,0,2},
			{2,0,0,2,0,2,0,0,0,2,2,0,0,0,0,2,0,0,0,2},
			{2,0,2,2,0,2,0,2,0,2,2,0,2,2,0,2,0,2,0,2},
			{2,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,2,0,2},
			{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2}
			
		};
		

			Ogre::Real posX = 0;
		Ogre::Real posZ = 0;
		for(int x = 0; x < size ; x++)
		{
			int my_intX = x+1;
			for(int i = 0; i < size ; i++)
			{
				Ogre::String name = "CubeX ";
				
				int my_intY = i+1;
				name.append(Ogre::StringConverter::toString(my_intX));
				name.append(" Y ");
				name.append(Ogre::StringConverter::toString(my_intY));
				_myCubesWaypoints[x][i] = std::make_pair(Ogre::Vector3(posX,0,posZ),0);
				if(_myCubesBool[x][i]> 0)
				{
					_myCubes[x][i] =  _sceneManager->createEntity(name, "Cube.mesh");
					_myCubes[x][i]->setMaterialName("shader/texture3");
					//_myCubes[x][i]->setMaterialName("shader/orange");
					//const Ogre::AxisAlignedBox& CubeBoundingBox=_myCubes[x][i]->getWorldBoundingBox(true);
						

					Ogre::SceneNode* cubenodeX = _sceneManager->getRootSceneNode()->createChildSceneNode();
				//cubenode2->attachObject(_myCube2);
					cubenodeX->scale(0.1, (_myCubesBool[x][i]*0.1), 0.1);    //Try different values
					cubenodeX->setPosition(posX, 0.0, posZ);
					//cubenodeX->showBoundingBox(true);
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
			posX = 0;
			posZ += 10;
		}

			//print height map
			for(int x = 0; x < size ; x++)
			{
				std::cout << "{";
				for(int i = 0; i < size ; i++)
				{
					std::cout << _myCubesWaypoints[x][i].second << ",";
				}
				std::cout << "}"<< std::endl;
			}

		}

		void walkPath()
		{

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
			world->setMaterialName("shader/texture_ground");
			Ogre::SceneNode* modelNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
			modelNode->attachObject(world);
			cout << "created model scene node\n";
			
			modelNode->setPosition(0,-5,0);
			modelNode->setScale(100,1,100); // You may have to scale your object to see it well
			//modelNode->attachObject(world);
			cout << "attached world model to scene node\n";

			Ogre::Entity* sinbad = _sceneManager->createEntity("sinbad", "Sinbad.mesh");
			markerNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
			markerNode->attachObject(sinbad);
			//markerNode->setPosition(modelNode->getPosition());
			/*
			Ogre::Entity* marker2 = _sceneManager->createEntity("sinbad2", "Sinbad.mesh");
			Ogre::SceneNode* markerNode2 = _sceneManager->getRootSceneNode()->createChildSceneNode();
			markerNode2->attachObject(_sceneManager->getEntity("sinbad2"));
			markerNode2->setPosition(farRightCorner);
			*/
			/* 
			hokay so over x and z double loop
			*/

			//ugly but working world/height map generation 
	
	
		renderMaze();
		renderHeightmap();
		//generate maze and do raycast
		Ogre::Vector3 startPos;
			startPos = _myCubesWaypoints[0][1].first;
		markerNode->setPosition(startPos);

		mWalkList.push_back(_myCubesWaypoints[4][1].first);
		mWalkList.push_back(_myCubesWaypoints[4][4].first);
		mWalkList.push_back(_myCubesWaypoints[8][4].first);
		mWalkList.push_back(_myCubesWaypoints[8][6].first);
		mWalkList.push_back(_myCubesWaypoints[6][6].first);
		mWalkList.push_back(_myCubesWaypoints[6][8].first);
		mWalkList.push_back(_myCubesWaypoints[9][8].first);


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
};