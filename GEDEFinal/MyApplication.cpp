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
			
			Ogre::Camera* camera = _sceneManager->createCamera("Camera");
			camera->setPosition(Ogre::Vector3(0, 200, 25));
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
			Ogre::SceneNode* modelNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
			cout << "created model scene node\n";
			modelNode->attachObject(world);
			cout << "attached world model to scene node\n";
			modelNode->setPosition(0,0,0);
			modelNode->setScale(100,0,100); // You may have to scale your object to see it well\\

			_sceneManager->setAmbientLight(Ogre::ColourValue(0.6f,0.6f,0.6f));
			Ogre::Light* light = _sceneManager->createLight("Light1");
			light->setType(Ogre::Light::LT_DIRECTIONAL);
			light->setDirection(Ogre::Vector3(1, -1, 0));

			_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		}
};