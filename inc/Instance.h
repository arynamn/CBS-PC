#pragma once

#include "common.h"

typedef std::pair<int, int> event;

#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H
struct Cell
{
    int x;
    int y;

    Cell(int x=0, int y=0) : x(x), y(y) {}
    bool operator<(const Cell &o) const {
        return (this->x < o.x) || (this->x == o.x && this->y < o.y);
    }
};

struct TempConstraint 
{
    Cell from_loc;
    Cell to_loc;
    int from_landmark;
    int to_landmark;
    TempConstraint(Cell m_from_loc, Cell m_to_loc, int m_from_lm, int m_to_lm) :
        from_loc(m_from_loc),
        to_loc(m_to_loc),
        from_landmark(m_from_lm),
        to_landmark(m_to_lm)
    {}
};
#endif

// Currently only works for undirected unweighted 4-neighbor grids
class Instance
{
public:
	// int moves_offset[MOVE_COUNT];
	vector<bool> my_map;
	string map_fname;
	string agent_fname;

	int num_of_agents;
	vector<int> start_locations;
	vector<vector<int>> goal_locations;
	vector<vector<int>> deadlines;

	bool loadMap();
	void printMap() const;
	void saveMap() const;

	virtual bool loadAgents();
	virtual void saveAgents() const;

	void generateConnectedRandomGrid(int rows, int cols, int obstacles); 	// initialize new [rows x cols] map with random obstacles
	void generateRandomAgents(int warehouse_width);
	bool addObstacle(int obstacle);		   									// add this obstacle only if the map is still connected
	bool isConnected(int start, int goal); 									// run BFS to find a path between start and goal, return true if a path exists.

	int randomWalk(int loc, int steps) const;

	// Class SingleAgentSolver can access private members of Node
	friend class SingleAgentSolver;

	int num_of_cols;
	int num_of_rows;
	int map_size;
	int num_of_constraints;
	vector<int> goals_count;

	Instance() {}
	Instance(
		const string &map_fname, 
		const string &agent_fname,
		int num_of_agents = 0, 
		int num_of_rows = 0, 
		int num_of_cols = 0, 
		int num_of_obstacles = 0, 
		int warehouse_width = 0
	);
	Instance(
		std::vector<std::vector<int>> m_my_map,
		std::vector<std::vector<int>> m_start_locations,
		std::vector<std::vector<std::vector<int>>> m_goal_locations,
		std::vector<std::vector<int>> deadlines,
		std::vector<std::vector<int>> m_temp_constraints,
		int screen=0
	);

	void printAgents() const;

	inline bool isObstacle(int loc) const { return my_map[loc]; }

	inline bool validMove(int curr, int next) const;
	list<int> getNeighbors(int curr) const;

	inline int linearizeCoordinate(int row, int col) const { return (this->num_of_cols * row + col); }
	inline int getRowCoordinate(int id) const { return id / this->num_of_cols; }
	inline int getColCoordinate(int id) const { return id % this->num_of_cols; }
	inline pair<int, int> getCoordinate(int id) const { return make_pair(id / this->num_of_cols, id % this->num_of_cols); }
	inline int getCols() const { return num_of_cols; }

	inline int getManhattanDistance(int loc1, int loc2) const
	{
		int loc1_x = getRowCoordinate(loc1);
		int loc1_y = getColCoordinate(loc1);
		int loc2_x = getRowCoordinate(loc2);
		int loc2_y = getColCoordinate(loc2);
		return abs(loc1_x - loc2_x) + abs(loc1_y - loc2_y);
	}

	inline int getManhattanDistance(const pair<int, int> &loc1, const pair<int, int> &loc2) const
	{
		return abs(loc1.first - loc2.first) + abs(loc1.second - loc2.second);
	}

	int getDegree(int loc) const
	{
		assert(loc >= 0 && loc < map_size && !my_map[loc]);
		int degree = 0;
		if (0 < loc - num_of_cols && !my_map[loc - num_of_cols])
			degree++;
		if (loc + num_of_cols < map_size && !my_map[loc + num_of_cols])
			degree++;
		if (loc % num_of_cols > 0 && !my_map[loc - 1])
			degree++;
		if (loc % num_of_cols < num_of_cols - 1 && !my_map[loc + 1])
			degree++;
		return degree;
	}

	int getDefaultNumberOfAgents() const { return num_of_agents; }

	// should be moved to private
	// vector<TemporalEdge> temporal_cons;
	// temporal_cons[i * num_of_agents + j] = [{k, l}]
	// The k-th task of i should happens before the l-th task of j
	vector<vector<pair<int, int>>> temporal_cons;
};
