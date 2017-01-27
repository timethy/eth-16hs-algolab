#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

using namespace std;

// choose exact floating-point type
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef long num;
typedef vector<vector<num>> vvec;

inline bool too_small(num C, num V, vvec& v, vector<num> c, vector<num> r, num R) {
	Program qp (CGAL::SMALLER, true, 0, false, 0);
	int COST = 0, REWARD = 1;

	for(unsigned i = 0; i < r.size(); i++) {
		qp.set_a(i, REWARD, r[i]);
	}
	qp.set_r(REWARD, CGAL::LARGER);
	qp.set_b(REWARD, R);

	for(unsigned i = 0; i < c.size(); i++) {
		qp.set_a(i, COST, c[i]);
	}
	qp.set_b(COST, C);
	// Objective function: Variance min
	// we need to specify the entries of the symmetric matrix 2D, on and below the diagonal
	for(unsigned i = 0; i < r.size(); i++) {
		for(unsigned j = 0; j <= i; j++) {
			qp.set_d(i, j, 2*v[i][j]);
		}
	}

	Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
	//assert (s.solves_quadratic_program(qp));

	//cout << s << endl;
	//cout << s.objective_value() << "<?=" << V << endl;

	return !s.is_infeasible() && s.objective_value() <= ET(V);
}

bool testcase() {
	unsigned n, m;
	cin >> n >> m;

	if(n == 0 && m == 0) {
		// In theory: cout << 0 << endl;
		return true;
	}

	vector<num> c(n), r(n);
	vvec v(n, vector<num>(n));
	vector<num> C(m), V(m);

	for(unsigned i = 0; i < n; i++) {
		cin >> c[i] >> r[i];
	}
	for(unsigned i = 0; i < n; i++) {
		for(unsigned j = 0; j < n; j++) {
			cin >> v[i][j];
		}
	}
	for(unsigned i = 0; i < m; i++) {
		cin >> C[i] >> V[i];
	}

	for(unsigned i = 0; i < m; i++) {
		num a = 0, b = 2<<8;
		//cout << "B=" << b << endl;
		while(too_small(C[i], V[i], v, c, r, b)) {
			b *= 2;
			//cout << "B=" << b << endl;
		}
		while(a != b) {
			auto k = (a+b+1)/2;
			//cout << "K=" << k << endl;
			if(too_small(C[i], V[i], v, c, r, k)) {
				a = k;
			} else {
				b = k-1;
			}
		}
		cout << a << endl;
	}
	return false;
}

int main() {
	while(!testcase());
	return 0;
}

