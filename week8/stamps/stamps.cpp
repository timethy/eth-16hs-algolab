// for algolab, by taubnert@ethz.ch
#include <climits>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Gmpq ET;

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
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

	vector<vector<unsigned>> illuminated(s);
	for (unsigned j = 0; j < s; j++) {
		for (unsigned i = 0; i < l; i++) {
			bool blocked = false;
			for (unsigned k = 0; k < w; k++) {
				Segment lamp_stamp = Segment(lamps[i], stamps[j]);
				if(CGAL::do_intersect(walls[k], lamp_stamp)) {
					blocked = true;
					break;
				}
			}
			if (!blocked) {
				illuminated[j].push_back(i);
			}
		}
	}

	// Construct the linear program Ax <= b, x = [I_j]
	Program lp(CGAL::SMALLER, true, 1, true, 4096);
	for (unsigned j = 0; j < s; j++) {
		for (auto it = illuminated[j].begin(); it != illuminated[j].end(); ++it) {
			unsigned i = *it;
			auto r2 = CGAL::squared_distance(lamps[i], stamps[j]);
			lp.set_a(i, 2*j,   1.0/CGAL::to_double(r2));
			lp.set_a(i, 2*j+1, 1.0/CGAL::to_double(r2));
		}
		lp.set_b(2*j,   M[j]);
		lp.set_b(2*j+1, 1);
		lp.set_r(2*j+1, CGAL::LARGER);

		lp.set_c(j, 1);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if(sol.is_infeasible()) {
		cout << "no" << endl;
	} else {
		cout << "yes" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
