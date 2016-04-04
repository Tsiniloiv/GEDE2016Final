#include "OGRE/Ogre.h"
#include "State.cpp"
#include "OIS\OIS.h"
#include "SearchNode.cpp"

using namespace std;

class Agent {
	/* 
		Member variables: current state, current path
		Methods: search, return next destination
	*/
	private:
		State currState;
		std::deque<Ogre::Vector3> currSolution;
	public:
		Agent(State s) {
			currState = s;
		}
		void search() {
			SearchNode root = SearchNode(currState, nullptr, actions::none, 0);


		}
};