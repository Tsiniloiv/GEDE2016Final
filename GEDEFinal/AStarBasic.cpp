#include "OGRE/Ogre.h"
#include "OIS\OIS.h"
#include "SearchNode.cpp"
#include "ISearch.cpp"
#include <queue>
#include <map>

using namespace std;

class AStarBasic : ISearch {
	private:
		SearchNode root;
		priority_queue<SearchNode, SearchNode::cmp> frontier;
		map<State, SearchNode> marked;
	public:
		AStarBasic() {
		}
};