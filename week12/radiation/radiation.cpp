#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <bitset>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

using namespace std;

typedef CGAL::Gmpzf ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

inline vector<double> coeff(int d, double x, double y, double z) {
	auto v_new = vector<double>();
	for(int ix = 0; ix <= d; ix++) {
		for(int iy = 0; iy <= d-ix; iy++) {
			for(int iz = 0; iz <= d-ix-iy; iz++) {
				v_new.push_back(pow(x,ix)*pow(y,iy)*pow(z,iz));
			}
		}
	}
	return v_new;
}

inline bool too_small(int d,
		vector<double> x_h, vector<double> y_h, vector<double> z_h,
		vector<double> x_t, vector<double> y_t, vector<double> z_t) {
	int h = x_h.size();
	int t = x_t.size();

	if(d == 0) {
		return h > 0 && t > 0;
	}

	int eps = 0;
	Program lp(CGAL::SMALLER, false, 0, false, 0);
	for (int i = 0; i < h; ++i) {
		auto co = coeff(d, x_h[i], y_h[i], z_h[i]);
		//cout << "h" << i << ": ";
		for(unsigned j = 0; j < co.size(); j++) {
			lp.set_a(j+1, i, co[j]);
		//	cout << co[j] << " ";
		}
		//cout << endl;
		// epsilon
		lp.set_a(eps, i, 1);
		lp.set_b(i, 0);
	}
	for (int i = 0; i < t; ++i) {
		auto co = coeff(d, x_t[i], y_t[i], z_t[i]);
		//cout << "t" << i << ": ";
		for(unsigned j = 0; j < co.size(); j++) {
			lp.set_a(j+1, i+h, co[j]);
		//	cout << co[j] << " ";
		}
		//cout << endl;
		// epsilon
		lp.set_a(eps, i+h, -1);
		lp.set_b(i+h, 0);
		lp.set_r(i+h, CGAL::LARGER);
	}

	// maximize eps
	lp.set_c(eps, -1);
	lp.set_l(eps, true, 0); // eps >= 0
	lp.set_u(eps, true, 1); // eps >= 0

	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_linear_program(lp, ET(), options);

	//cout << s << endl;
	return s.is_infeasible() || (s.objective_value() == 0);
}

void testcase() {
	int h, t;
	cin >> h >> t;

	int d_max = 30;

	vector<double> x_h(h), y_h(h), z_h(h);
	vector<double> x_t(t), y_t(t), z_t(t);

	for (int i = 0; i < h; i++) {
		cin >> x_h[i] >> y_h[i] >> z_h[i];
	}
	for (int i = 0; i < t; i++) {
		cin >> x_t[i] >> y_t[i] >> z_t[i];
	}
	int a = 0;
	int b = 1;
	while(b < d_max && too_small(b, x_h, y_h, z_h, x_t, y_t, z_t)) {
		a = b+1;
		b *= 2;
	}
	b = min(b, d_max)+1;
	while(a != b) {
		auto k = (a+b)/2;
		//cout << "k=" << k << endl;
		if(too_small(k, x_h, y_h, z_h, x_t, y_t, z_t)) {
			a = k+1;
		} else {
			b = k;
		}
	}
	if(a > d_max) {
		cout << "Impossible!" << endl;
	} else {
		cout << a << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while (T--) testcase();
	return 0;
}

