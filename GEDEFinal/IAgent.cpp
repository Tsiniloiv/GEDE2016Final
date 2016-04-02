#include "OGRE/Ogre.h"
#include "State.cpp"
#include "OIS\OIS.h"

using namespace std;

class IAgengt {
	/* 
		This'll be a virtual class which the real agents will inherit. Polymorphism woo!

		Member variables: current state, current path
		Methods: search, generate successor state, heuristic
	*/
	private:
		State currState;
		std::deque<Ogre::Vector3> currSolution;
	public:
		
};