#include "OGRE/Ogre.h"
#include "State.cpp"


using namespace std;

class SearchNode {
	/* 
		TODO: Hold world state, parent node
	*/
	public:
		State state;
		SearchNode* parent;
};