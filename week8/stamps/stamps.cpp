// for algolab, by taubnert@ethz.ch
#include <climits>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Gmpz ET;

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution <ET> Solution;

typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

void testcase() {
	unsigned l, s, w;
	cin >> l >> s >> w;

	vector<Point> lamps(l);
	for (unsigned i = 0; i < l; i++) {
		cin >> lamps[i];
	}
	vector<Point> stamps(s);
	vector<int> M(s);
	for (unsigned i = 0; i < s; i++) {
		cin >> stamps[i] >> M[i];
	}
	vector<Segment> walls(w);
	for (unsigned i = 0; i < w; i++) {
		Point start, end;
		cin >> start >> end;
		walls[i] = Segment(start, end);
	}

	vector<unsigned> illuminated(s);
	for (unsigned i = 0; i < l; i++) {
		for (unsigned j = 0; j < s; j++) {
			for (unsigned k = 0; k < w; k++) {
				Segment lamp_stamp = Segment(lamps[i], stamps[j]);
				if (!CGAL::do_intersect(walls[k], lamp_stamp)) {
					illuminated[j].push_back(i);
				}
			}
		}
	}

	// Construct the linear program Ax <= b, x = [I_j]
	Program lp(CGAL::SMALLER, true, 0, false, 0);
	for (unsigned j = 0; j < s; j++) {
		for (auto it = illuminated[j].begin(); it != illuminated[j].end(); ++it) {
			unsigned i = *it;
			auto r2 = CGAL::squared_distance(lamps[i], stamps[j]);
			lp.set_a(i, j, 1/ET(r2));
		}
		lp.set_b(j, M[j]);
		// Constraint I_j >= 1
		lp.set_l(j, true, 1);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if(sol.is_infeasible()) {
		cout << "no" << endl;
	} else {
		cout << "yes" << endl;
	}

	cout << 0 << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
