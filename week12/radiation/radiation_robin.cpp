#include <iostream>
#include <vector>

#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

// choose exact floating-point type
typedef CGAL::Gmpzf ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int DMAX = 30;

using namespace std;

vector<double> coeff(double x, double y, double z, int d) {
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

bool to_small(int d, vector<double> hx, vector<double> hy, vector<double> hz, vector<double> tx, vector<double> ty, vector<double> tz) {
    int h = hx.size();
    int t = tx.size();
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    for (int i = 0; i < h; i++) {
        vector<double> vals = coeff(hx.at(i), hy.at(i), hz.at(i), d);
        for (int j = 0; j < vals.size(); j++) {
            lp.set_a(j, i, vals.at(j));
        }
        lp.set_r(i, CGAL::SMALLER);
        lp.set_b(i, -1.0);
    }
    for (int i = 0; i < t; i++) {
        vector<double> vals = coeff(tx.at(i), ty.at(i), tz.at(i), d);
        for (int j = 0; j < vals.size(); j++) {
            lp.set_a(j, h+i, vals.at(j));
        }
        lp.set_b(h+i, 1.0);
        lp.set_r(h+i, CGAL::LARGER);
    }
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);

    return s.is_infeasible();
}

void solve() {
    int h, t; // healthy, tumor
    cin >> h >> t;

    vector<double> hx(h,0);
    vector<double> hy(h,0);
    vector<double> hz(h,0);
    for (int i = 0; i < h; i++) {
        cin >> hx.at(i);
        cin >> hy.at(i);
        cin >> hz.at(i);
    }
    vector<double> tx(t,0);
    vector<double> ty(t,0);
    vector<double> tz(t,0);
    for (int i = 0; i < t; i++) {
        cin >> tx.at(i);
        cin >> ty.at(i);
        cin >> tz.at(i);
    }

    if(h == 0 || t == 0) {
        cout << 0 << endl;
        return;
    }

    int lmin=1,lmax = 2;
    while (to_small(lmax,hx, hy, hz, tx, ty, tz)) {
        if (lmax > DMAX) {
            break;
        }
        lmax = min(2*lmax, DMAX+1);
    }

    while (lmin != lmax) {
        int p = (lmin+lmax)/2;
        if (to_small(p,hx, hy, hz, tx, ty, tz))
            lmin = p+1;
        else {
            lmax = p;
        }
    }
    int L = lmin;

    if (L > DMAX) {
        cout << "Impossible!" << endl;
    } else {
        cout << L << endl;
    }
	/*
	int a = 0;
	int b = 1;
	while(b < DMAX && to_small(b, hx, hy, hz, tx, ty, tz)) {
		b *= 2;
	}
	b = min(b, DMAX)+1;
	while(a != b) {
		auto k = (a+b)/2;
		//cout << "k=" << k << endl;
		if(to_small(k, hx, hy, hz, tx, ty, tz)) {
			a = k+1;
		} else {
			b = k;
		}
	}
	if(a > DMAX) {
		cout << "Impossible!" << endl;
	} else {
		cout << a << endl;
	}*/
}


int main() {
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        solve();
    }
    return 0;
}
