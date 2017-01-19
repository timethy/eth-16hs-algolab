// for algolab, by taubnert@ethz.ch
#include <climits>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz ET;

#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;

using namespace std;

typedef long long dist;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution <ET> Solution;

dist distance(dist x1, dist y1, dist x2, dist y2) {
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

void testcase() {
	unsigned a, s, b, e;
	cin >> a >> s >> b;
	cin >> e;

	vector <dist> p_x(a), p_y(a), rho(a);
	for (unsigned i = 0; i < a; i++) {
		cin >> p_x[i] >> p_y[i] >> rho[i];
	}
	vector <dist> s_x(s), s_y(s);
	for (unsigned i = 0; i < s; i++) {
		cin >> s_x[i] >> s_y[i];
	}
	vector <dist> b_x(b), b_y(b);
	for (unsigned i = 0; i < b; i++) {
		cin >> b_x[i] >> b_y[i];
	}

	vector <vector<unsigned>> p_affected(a);
	// Find affectable for every shooting position
	for (unsigned i = 0; i < s; i++) {
		// Find nearest bounty hunter
		dist dist2 = LLONG_MAX;
		for (unsigned j = 0; j < b; j++) {
			dist d = distance(b_x[j], b_y[j], s_x[i], s_y[i]);
			dist2 = min(dist2, d);
		}
		for (unsigned j = 0; j < a; j++) {
			dist d = distance(p_x[j], p_y[j], s_x[i], s_y[i]);
			if (d < dist2) {
				p_affected[j].push_back(i);
			}
		}
	}

	bool infeasible = false;
	// by default, we have a nonnegative LP with Ax >= b
	Program lp(CGAL::LARGER, true, 0, false, 0);
	// now set the non-default entries
	for (unsigned i = 0; i < a; i++) {
		if (p_affected[i].size() == 0) {
			infeasible = true;
			break;
		}
		for (auto it = p_affected[i].begin(); it != p_affected[i].end(); ++it) {
			unsigned j = *it;
			dist d = distance(p_x[i], p_y[i], s_x[j], s_y[j]);
			if (d <= 1) {
				d = 1;
			}
			double d_inv = 1.0 / ((double) d);
			lp.set_a(j, i, d_inv);
		}
		lp.set_b(i, rho[i]);
	}
	if(!infeasible) {
		for (unsigned i = 0; i < s; i++) {
			lp.set_c(i, 1);
		}

		Solution sol = CGAL::solve_linear_program(lp, ET());
//	assert(sol.solves_linear_program(lp));

		if (!sol.is_infeasible() && to_double(sol.objective_value()) <= e) {
			cout << "y" << endl;
		} else {
			cout << "n" << endl;
		}
	} else {
		cout << "n" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}

