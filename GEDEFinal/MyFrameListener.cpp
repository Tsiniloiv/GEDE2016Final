#include "OGRE/Ogre.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreMesh.h"
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
		Ogre::Vector3 _Direction;
		Ogre::Vector3 _Destination;
		Ogre::SceneNode* _Node;
		Ogre::Real _Distance;
		Ogre::Real _WalkSpd;
		std::deque<Ogre::Vector3> _WalkList;
	public: 
 		MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,std::deque<Ogre::Vector3> mWalkList,Ogre::SceneNode* node) 
		{
			OIS::ParamList parameters;
			unsigned int windowHandle = 0;
			std::ostringstream windowHandleString;
			win->getCustomAttribute("WINDOW", &windowHandle);
			windowHandleString << windowHandle;
			parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
			_InputManager = OIS::InputManager::createInputSystem(parameters);
			_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject(OIS::OISKeyboard, false));
			_Cam = cam;
			_Node = node;
			_movementspeed = 50.0f;
			_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject(OIS::OISMouse, false));
			_Direction = Ogre::Vector3::ZERO;
			_Destination = Ogre::Vector3::ZERO;
			_WalkSpd = 5;
			_Distance = 0;
			_WalkList = mWalkList;
		}
		~MyFrameListener() {
			_InputManager->destroyInputObject(_Keyboard);
			_InputManager->destroyInputObject(_Mouse);
			OIS::InputManager::destroyInputSystem(_InputManager);
		}

		bool nextLocation()
		{
			if (_WalkList.empty())
				return false;
 
			_Destination = _WalkList.front();
			_WalkList.pop_front();
			_Direction = _Destination - _Node->getPosition();
			_Distance = _Direction.normalise();
 
  return true;
}

		bool frameStarted(const Ogre::FrameEvent& evt) 
		{
			_Keyboard->capture();
			if(_Keyboard->isKeyDown(OIS::KC_ESCAPE)) {
				return false;
			}
			Ogre::Vector3 translate(0, 0, 0);
			if(_Keyboard->isKeyDown(OIS::KC_W)) {
				translate += Ogre::Vector3(0, 0, -1);
			}
			if(_Keyboard->isKeyDown(OIS::KC_S)) {
				translate += Ogre::Vector3(0, 0, 3);
			}
			if(_Keyboard->isKeyDown(OIS::KC_A)) {
				translate += Ogre::Vector3(-1, 0, 0);
			}
			if(_Keyboard->isKeyDown(OIS::KC_D)) {
				translate += Ogre::Vector3(1, 0, 0);
			}
			if(_Keyboard->isKeyDown(OIS::KC_Q)) {
				translate += Ogre::Vector3(0, -1, 0);
			}
			if(_Keyboard->isKeyDown(OIS::KC_E)) {
				translate += Ogre::Vector3(0, 1, 0);
			}
			_Cam->moveRelative(translate * evt.timeSinceLastFrame * _movementspeed);

			//path movement

			if (_Direction == Ogre::Vector3::ZERO) 
			{
				if (nextLocation())
				{
					/*
					mAnimationState = mEntity->getAnimationState("Walk");
					mAnimationState->setLoop(true);
					mAnimationState->setEnabled(true);
					*/
				}
			}
			else
			{
    Ogre::Real move = _WalkSpd * evt.timeSinceLastFrame;
    _Distance -= move;
 
    if (_Distance <= 0.0)
    {
      _Node->setPosition(_Destination);
      _Direction = Ogre::Vector3::ZERO;
 
      if (nextLocation())
      {
		Ogre::Vector3 src = _Node->getOrientation() * Ogre::Vector3::UNIT_X;
 
		if ((1.0 + src.dotProduct(_Direction)) < 0.0001)
		{
			_Node->yaw(Ogre::Degree(180));
		}
		else
		{
			Ogre::Quaternion quat = src.getRotationTo(_Direction);
			_Node->rotate(quat);
		}
      }
      else
      {
		  /*
		_AnimationState = _Entity->getAnimationState("Idle");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		*/
      }
    }
    else
    {
      _Node->translate(move * _Direction);
    }
  }

			//

			/*_Mouse->capture();
			float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
			float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
			_Cam->yaw(Ogre::Radian(rotX));
			_Cam->pitch(Ogre::Radian(rotY));*/
			return true;
		}
};