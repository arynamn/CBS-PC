#pragma once

#include "MDD.h"

class RectangleReasoning
{

private:
	const Instance &instance;

	shared_ptr<Conflict> findRectangleConflictByRM(
		const vector<Path *> &paths, int timestep, int a1, int a2, const MDD *mdd1, const MDD *mdd2);
	shared_ptr<Conflict> findRectangleConflictByGR(
		const vector<Path *> &paths, int timestep, int a1, int a2, const MDD *mdd1, const MDD *mdd2);

	bool ExtractBarriers(const MDD &mdd, int loc, int timestep, int dir, int dir2, int start, int goal, int start_time, list<Constraint> &B);
	bool isEntryBarrier(const Constraint &b1, const Constraint &b2, int dir1);
	bool isExitBarrier(const Constraint &b1, const Constraint &b2, int dir1);
	bool blockedNodes(const Path &path, const pair<int, int> &Rs, const pair<int, int> &Rg, int Rg_t, int dir);
	bool isCut(const Constraint b, const pair<int, int> &Rs, const pair<int, int> &Rg);
	pair<int, int> getIntersection(const Constraint &b1, const Constraint &b2);

	void generalizedRectangle(
		const Path &path1, 
		const Path &path2, 
		const MDD &mdd1, 
		const MDD &mdd2,
		const list<Constraint> &B1, 
		const list<Constraint> &B2, 
		int timestep,
		int &best_type, 
		pair<int, int> &best_Rs, 
		pair<int, int> &best_Rg
	);

	// Identify rectangle conflicts
	bool isRectangleConflict(
		const pair<int, int> &s1, 
		const pair<int, int> &s2,
		const pair<int, int> &g1, 
		const pair<int, int> &g2, 
		int g1_t, int g2_t
	);										// for CR and R
	bool isRectangleConflict(
		const pair<int, int> &s1, 
		const pair<int, int> &s2, 
		const pair<int, int> &g1, 
		const pair<int, int> &g2
	) const; // for RM

	// Classify rectangle conflicts
	int classifyRectangleConflict(
		const pair<int, int> &s1, 
		const pair<int, int> &s2,
		const pair<int, int> &g1, 
		const pair<int, int> &g2
	); // for CR and R
	int classifyRectangleConflict(
		const pair<int, int> &s1, 
		const pair<int, int> &s2, 
		const pair<int, int> &g1, 
		const pair<int, int> &g2,
		const pair<int, int> &Rg
	); // for RM

	// Compute rectangle corners
	pair<int, int> getRg(const pair<int, int> &s1, const pair<int, int> &g1, const pair<int, int> &g2);
	pair<int, int> getRs(const pair<int, int> &s1, const pair<int, int> &s2, const pair<int, int> &g1);

	// Compute start and goal candidates for RM
	list<int> getStartCandidates(const Path &path, int timestep);
	list<int> getGoalCandidates(const Path &path, int timestep);

	// Compute start and goal candidates for GR
	int getStartCandidate(const Path &path, int dir1, int dir2, int timestep);
	int getGoalCandidate(const Path &path, int dir1, int dir2, int timestep);

	// int getRectangleTime(const Conflict& conflict, const std::vector<std::vector<PathEntry>*>& paths, int num_col);
	bool hasNodeOnBarrier(const MDD *mdd, int y_start, int y_end, int x, int t_min, bool horizontal) const;

	bool addModifiedBarrierConstraints(
		int a1, 
		int a2, 
		const pair<int, int> &Rs, 
		const pair<int, int> &Rg,
		const pair<int, int> &s1, 
		const pair<int, int> &s2, int Rg_t,
		const MDD *mdd1, 
		const MDD *mdd2,
		list<Constraint> &constraint1, 
		list<Constraint> &constraint2
	); // for RM

	// add a horizontal modified barrier constraint
	bool addModifiedHorizontalBarrierConstraint(
		int agent, const MDD *mdd, int x, int Ri_y, int Rg_y, int Rg_t, list<Constraint> &constraints);

	// add a vertical modified barrier constraint
	bool addModifiedVerticalBarrierConstraint(
		int agent, const MDD *mdd, int y, int Ri_x, int Rg_x, int Rg_t, list<Constraint> &constraints);

	bool blocked(const Path &path, const list<Constraint> &constraints);
	bool traverse(const Path &path, int loc, int t);

public:
	bool use_rectangle_reasoning;
	double accumulated_runtime = 0;

	RectangleReasoning(const Instance &instance) : instance(instance) {}

	shared_ptr<Conflict> run(
		const vector<Path *> &paths, 
		int timestep,
		int a1, 
		int a2, 
		const MDD *mdd1, 
		const MDD *mdd2
	);
};
