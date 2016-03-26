#include "OGRE/Ogre.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreEntity.h"
#include "OIS\OIS.h"

class MyFrameListener : public Ogre::FrameListener {
	private: 
		OIS::InputManager* _InputManager;
		OIS::Keyboard* _Keyboard;
		OIS::Mouse* _Mouse;
		Ogre::Camera* _Cam;
		float _movementspeed;
	public: 
		MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam) {
			OIS::ParamList parameters;
			unsigned int windowHandle = 0;
			std::ostringstream windowHandleString;
			win->getCustomAttribute("WINDOW", &windowHandle);
			windowHandleString << windowHandle;
			parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
			_InputManager = OIS::InputManager::createInputSystem(parameters);
			_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject(OIS::OISKeyboard, false));
			_Cam = cam;
			_movementspeed = 50.0f;
			_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject(OIS::OISMouse, false));

		}
		~MyFrameListener() {
			_InputManager->destroyInputObject(_Keyboard);
			_InputManager->destroyInputObject(_Mouse);
			OIS::InputManager::destroyInputSystem(_InputManager);
		}
		bool frameStarted(const Ogre::FrameEvent& evt) {
			_Keyboard->capture();
			if(_Keyboard->isKeyDown(OIS::KC_ESCAPE)) {
				return false;
			}
			Ogre::Vector3 translate(0, 0, 0);
			if(_Keyboard->isKeyDown(OIS::KC_W)) {
				translate += Ogre::Vector3(0, 0, -1);
			}
			if(_Keyboard->isKeyDown(OIS::KC_S)) {
				translate += Ogre::Vector3(0, 0, 1);
			}
			if(_Keyboard->isKeyDown(OIS::KC_A)) {
				translate += Ogre::Vector3(-1, 0, 0);
			}
			if(_Keyboard->isKeyDown(OIS::KC_D)) {
				translate += Ogre::Vector3(1, 0, 0);
			}
			_Cam->moveRelative(translate * evt.timeSinceLastFrame * _movementspeed);
			/*_Mouse->capture();
			float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
			float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
			_Cam->yaw(Ogre::Radian(rotX));
			_Cam->pitch(Ogre::Radian(rotY));*/
			return true;
		}
};