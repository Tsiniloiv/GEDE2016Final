#include "RedBlobAStarImplementation.cpp"

class RedBlobAStar {
	inline int heuristic(SquareGrid::Location a, SquareGrid::Location b) {
	  int x1, y1, x2, y2;
	  tie (x1, y1) = a;
	  tie (x2, y2) = b;
	  return abs(x1 - x2) + abs(y1 - y2);
	}

	template<typename Graph>
	void a_star_search
	  (const Graph& graph,
	   typename Graph::Location start,
	   typename Graph::Location goal,
	   unordered_map<typename Graph::Location, typename Graph::Location>& came_from,
	   unordered_map<typename Graph::Location, int>& cost_so_far)
	{
	  typedef typename Graph::Location Location;
	  PriorityQueue<Location> frontier;
	  frontier.put(start, 0);

	  came_from[start] = start;
	  cost_so_far[start] = 0;
  
	  while (!frontier.empty()) {
		auto current = frontier.get();

		if (current == goal) {
		  break;
		}

		for (auto next : graph.neighbors(current)) {
		  int new_cost = cost_so_far[current] + graph.cost(current, next);
		  if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
			cost_so_far[next] = new_cost;
			int priority = new_cost + heuristic(next, goal);
			frontier.put(next, priority);
			came_from[next] = current;
		  }
		}
	  }
	}

	int main() {
	  GridWithWeights grid = make_diagram4(); //world map
	  SquareGrid::Location start(1, 4); //starting location
	  SquareGrid::Location goal(8, 5); //goal location
	  unordered_map<SquareGrid::Location, SquareGrid::Location> came_from;
	  unordered_map<SquareGrid::Location, int> cost_so_far;
	  a_star_search(grid, start, goal, came_from, cost_so_far);
	  draw_grid(grid, 2, nullptr, &came_from);
	  std::cout << std::endl;
	  draw_grid(grid, 3, &cost_so_far, nullptr);
	  std::cout << std::endl;
	  vector<SquareGrid::Location> path = reconstruct_path(start, goal, came_from);
	  draw_grid(grid, 3, nullptr, nullptr, &path);
	}
};