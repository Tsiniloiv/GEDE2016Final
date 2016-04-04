#include "OGRE/Ogre.h"
#include "State.cpp"

using namespace std;

class SearchNode {
	public:
		State state;
		SearchNode* parent;
		actions action;
		int cost;
		int f;
		int g;
		int heuristic() {
			int deltaX = state.X - state.goalX;
			int deltaY = state.Y - state.goalY;

			return deltaX + deltaY;
		}

		SearchNode() {
			state = State();
		}

		SearchNode(State s, SearchNode* p, actions a, int c) {
			state = s;
			parent = p;
			action = a;
			cost = c;
			if(parent == nullptr) {
				g = 0;
			} else {
				g = parent->g + 1; // Action Cost = 1;
			}
			f = g + heuristic();
		}
};