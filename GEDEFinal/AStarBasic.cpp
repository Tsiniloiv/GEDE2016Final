#include "OGRE/Ogre.h"
#include "OIS\OIS.h"
#include "SearchNode.cpp"
#include "ISearch.cpp"
#include <queue>

using namespace std;

class AStarBasic : ISearch {
	private:
		SearchNode root;
		priority_queue<SearchNode> frontier;
	public:
		AStarBasic() {
		}
};