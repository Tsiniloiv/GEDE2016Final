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
#include <vector>
struct walker
{
	std::deque<Ogre::Vector3> mWalkList;
	Ogre::Entity* sinbad;
	Ogre::SceneNode* Node;
	int id;
	Ogre::Vector3 start;
	Ogre::Vector3 finish;
};

struct agent
{
	walker walk;
	Ogre::Vector3 _Direction;
	Ogre::Vector3 _Destination;
	Ogre::Real _Distance;
	bool Dancing;
	bool walking;
	Ogre::Quaternion OrgDirection;
	Ogre::AnimationState* mAnimationState;
	Ogre::AnimationState* _aniState;
	Ogre::AnimationState* _aniStateDance;
	Ogre::AnimationState* _aniStateTop;
	float _rotation;
};

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
		Ogre::Entity* _sinbad;
		Ogre::AnimationState* mAnimationState;
		Ogre::AnimationState* _aniState;
		Ogre::AnimationState* _aniStateDance;
		Ogre::AnimationState* _aniStateTop;
		bool Dancing;
		bool walking;
		std::deque<Ogre::Vector3> _WalkList;
		Ogre::Quaternion OrgDirection;
		agent* walkers;
		int agentNR;
	public: 
		//MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,std::deque<Ogre::Vector3> mWalkList,Ogre::SceneNode* node,Ogre::Entity* sinbad) 
		MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,walker agents[], int size) 
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
			agentNR = size;
			walkers = new agent[size];
			//_Node = node;
			for(int i = 0 ; i < size; i++)
			{
				walkers[i].walk = agents[i];
				walkers[i]._Direction = Ogre::Vector3::ZERO;
			walkers[i]._Destination = Ogre::Vector3::ZERO;
			walkers[i].walk.Node = agents[i].Node;
			
			walkers[i]._Distance = 0;
			walkers[i].Dancing = false;
			walkers[i].walking = false;
			/*
			walkers[i]._aniState = agents[i].sinbad->getAnimationState("RunBase");
			walkers[i]._aniStateDance = agents[i].sinbad->getAnimationState("Dance");
			walkers[i]._aniStateDance->setLoop(true);
			walkers[i]._aniState->setLoop(false);
			walkers[i]._aniStateTop = agents[i].sinbad->getAnimationState("RunTop");
			walkers[i]._aniStateTop->setLoop(false);
			*/
			walkers[i]._rotation = 0.0f;
			}

			for(int i = 0 ; i < size; i++)
			{
				/*
				walkers[i].OrgDirection = walkers[i].walk.Node->getOrientation();
				walkers[i]._aniState = walkers[i].walk.sinbad->getAnimationState("RunBase");
			walkers[i]._aniStateDance = walkers[i].walk.sinbad->getAnimationState("Dance");
			walkers[i]._aniStateDance->setLoop(true);
			walkers[i]._aniState->setLoop(false);
			walkers[i]._aniStateTop = walkers[i].walk.sinbad->getAnimationState("RunTop");
			walkers[i]._aniStateTop->setLoop(false);
			*/
			}

			//_Node = agents[0].Node;
			_movementspeed = 50.0f;
			_WalkSpd = 5;
			_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject(OIS::OISMouse, false));
			_Node = agents[0].Node;
			_Direction = Ogre::Vector3::ZERO;
			_Destination = Ogre::Vector3::ZERO;
			OrgDirection = _Node->getOrientation();
			_Distance = 0;
			_sinbad = agents[0].sinbad;
			_WalkList = agents[0].mWalkList;
			Dancing = false;
			walking = false;
			_aniState = agents[0].sinbad->getAnimationState("RunBase");
			_aniStateDance = agents[0].sinbad->getAnimationState("Dance");
			_aniStateDance->setLoop(true);
			_aniState->setLoop(false);
			_aniStateTop = agents[0].sinbad->getAnimationState("RunTop");
			_aniStateTop->setLoop(false);
			
		}
		~MyFrameListener() {
			_InputManager->destroyInputObject(_Keyboard);
			_InputManager->destroyInputObject(_Mouse);
			OIS::InputManager::destroyInputSystem(_InputManager);
		}

		bool nextLocation(int i) //Consider making this a query to the agent.
		{
			if (walkers[i].walk.mWalkList.empty())
				return false;
 
			walkers[i]._Destination = walkers[i].walk.mWalkList.front();
			walkers[i].walk.mWalkList.pop_front();
			walkers[i]._Direction = walkers[i]._Destination - walkers[i].walk.Node->getPosition();
			walkers[i]._Distance = walkers[i]._Direction.normalise();
 
  return true;
}

		bool frameStarted(const Ogre::FrameEvent& evt) 
		{
			float _rotation = 0.0f;
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
			//std::cout << "X " <<_Node->getPosition().x << " Y " << _Node->getPosition().y << " Z " << _Node->getPosition().z << std::endl;
			//path movement
			for(int i = 0; i < agentNR; i++)
			{
				if (walkers[i]._Direction == Ogre::Vector3::ZERO) 
				{
					if (nextLocation(i))
					{
						
						walkers[i].walking = true;
						
					}
				}
				else
				{
					walkers[i]._rotation = 0;
					Ogre::Real move = _WalkSpd * evt.timeSinceLastFrame;
					walkers[i]._Distance -= move;
	 
					if (walkers[i]._Distance <= 0.0)
					{
						walkers[i].walk.Node->setPosition(walkers[i]._Destination);
						walkers[i]._Direction = Ogre::Vector3::ZERO;
	 
						if (nextLocation(i))
						{
							Ogre::Vector3 src = walkers[i].walk.Node->getOrientation() * Ogre::Vector3::UNIT_X;
							Ogre::Vector3 tempPos = walkers[i].walk.Node->getPosition();
							if(walkers[i]._Direction.x == -1)
							{
								walkers[i]._rotation = -1.57f;
							}
							else if(walkers[i]._Direction.x == 0){}
							else
							{
								walkers[i]._rotation = 1.57f;
							}
							if(walkers[i]._Direction.z == 1)
							{
								walkers[i]._rotation = 0.00f;
							}
							else if(walkers[i]._Direction.z == 0){}
							else
							{
								walkers[i]._rotation = 3.14f;
							}
	
							walkers[i].walk.Node->resetOrientation();
							walkers[i].walk.Node->yaw(Ogre::Radian(walkers[i]._rotation));
							/*
						if ((1.0 + src.dotProduct(_Direction)) < 0.0001)
						{
							_Node->resetOrientation();
							_Node->yaw(Ogre::Degree(180));
						}
						else
						{
							_Node->resetOrientation();
							Ogre::Quaternion quat = src.getRotationTo(_Direction);
							_Node->rotate(quat);
						}
						*/
						}
						else
						{
							//_Node->rotate(OrgDirection);
							//walkers[i].walk.Node->setOrientation(walkers[i].OrgDirection);
							walkers[i].Dancing = true;
							walkers[i].walking = false;
						}
				}
				else
				{
					walkers[i].walk.Node->translate(move * walkers[i]._Direction);
					if(walkers[i].walk.Node->getPosition().y < walkers[i]._Destination.y)
					{
						Ogre::Vector3 temp = walkers[i].walk.Node->getPosition();
						temp.y += 0.4;
						walkers[i].walk.Node->setPosition(temp);
					}
				
				}
			}

			if(walkers[i].walking)
			{
				walkers[i].Dancing = false;
				/*
				walkers[i]._aniStateDance->setEnabled(false);
				walkers[i]._aniState->setEnabled (true) ;
				walkers[i]._aniStateTop->setEnabled (true) ;
				if (walkers[i]._aniState->hasEnded ( ) )
				{
					walkers[i]._aniState->setTimePosition( 0.0f ) ;
				}
				if (walkers[i]._aniStateTop->hasEnded ( ) )
				{
					walkers[i]._aniStateTop->setTimePosition( 0.0f ) ;
				}
				*/
				
			}
			else if(walkers[i].Dancing)
			{

				//walkers[i]._aniStateDance->setEnabled(true);
			}
			else
			{
				/*
				walkers[i]._aniState->setEnabled (false) ;
				walkers[i]._aniStateTop->setEnabled (false) ;
				walkers[i]._aniState->setTimePosition( 0.0f ) ;
				walkers[i]._aniStateTop->setTimePosition( 0.0f ) ;
				*/
			}
			/*
			walkers[i]._aniState->addTime(evt.timeSinceLastFrame) ;
			walkers[i]._aniStateDance->addTime(evt.timeSinceLastFrame) ;
			walkers[i]._aniStateTop->addTime(evt.timeSinceLastFrame) ;
			*/
		}
			//
		/*
			_Mouse->capture();
			float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
			float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
			_Cam->yaw(Ogre::Radian(rotX));
			_Cam->pitch(Ogre::Radian(rotY));
			*/
			return true;
		}
};