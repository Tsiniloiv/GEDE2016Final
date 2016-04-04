#include "OGRE/Ogre.h"
#include "OIS\OIS.h"
#include "SearchNode.cpp"

using namespace std;

class ISearch {
	/* 
		virtual class for implementing different search types
	*/
	private:
		std::deque<Ogre::Vector3> currSolution;
	public:
		virtual deque<Ogre::Vector3> search(SearchNode root);
};