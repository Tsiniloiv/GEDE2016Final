#include "OGRE/Ogre.h"
#include <list>

using namespace std;

enum actions {up, down, left, right, none};

class State {
	public:
		array<array<Ogre::Vector3, 20>, 20> world; //might have to be a double array of Vector3
		int X;
		int Y;//agent's coordinates within world waypoint map
		int goalX;
		int goalY;

		State() {
			X = 0;
			Y = 0;
			goalX = 9;
			goalY = 9;
		}

		State(array<array<Ogre::Vector3, 20>, 20> worldmap, int currX, int currY, int gX, int gY) {
			world = worldmap;
			X = currX;
			Y = currY;
			goalX = gX;
			goalY = gY;
		}

		bool isGoal() {
			if(X == goalX && Y == goalY) {
				return true;
			} else return false;
		}

		//need a thing to return legal actions
		list<actions> legalActions(Ogre::Real heightLimit) {
			/* needs the agent's height limit */

			list<actions> legal;

			Ogre::Real currentHeight = world[X][Y].y;
			Ogre::Real heightDiff;

			if(Y < 20) {
				Ogre::Real upHeight = world[X][Y+1].y;
				heightDiff = currentHeight - upHeight;
				if(heightDiff < 0) {
					heightDiff = -heightDiff;
				}
				if(heightDiff < heightLimit) {
					legal.push_back(actions::up);
				}
			}
			if(Y > 0) {
				Ogre::Real downHeight = world[X][Y-1].y;
				heightDiff = currentHeight - downHeight;
				if(heightDiff < 0) {
					heightDiff = -heightDiff;
				}
				if(heightDiff < heightLimit) {
					legal.push_back(actions::down);
				}
			}
			if(X < 20) {
				Ogre::Real rightHeight = world[X+1][Y].y;
				heightDiff = currentHeight - rightHeight;
				if(heightDiff < 0) {
					heightDiff = -heightDiff;
				}
				if(heightDiff < heightLimit) {
					legal.push_back(actions::right);
				}
			}
			if(X > 0) {
				Ogre::Real leftHeight = world[X+1][Y].y;
				heightDiff = currentHeight - leftHeight;
				if(heightDiff < 0) {
					heightDiff = -heightDiff;
				}
				if(heightDiff < heightLimit) {
					legal.push_back(actions::left);
				}
			}
			return legal;
		}

		/* Needs a thing to expand the state */
		State expandState(actions a) {
			if(a == actions::up) {
				Y = Y+1;
			}
			if(a == actions::down) {
				Y = Y-1;
			}
			if(a == actions::right) {
				X = X+1;
			}
			if(a == actions::left) {
				X = X-1;
			}
		}
};