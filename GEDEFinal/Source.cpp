

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreEntity.h"
#include "MyApplication.cpp"
 
int main()
 {
	 MyApplication* app = new MyApplication();

	 app->startup();
 
	return 0;
}

