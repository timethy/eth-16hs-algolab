// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  K::Point_2 Point;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

using namespace std;
Point P0;

int max_k(vector<Point> &P, unsigned a, unsigned b) {
	if(a + 1 == b) {
		return a;
	}
	unsigned n = P.size();
	unsigned k = (a+b)/2;
	auto r_k = squared_distance(P0, P[k]);

	auto r_rest = squared_distance(P0, P0);
	if(n - k > 1) {
		Min_circle mc(P.begin() + (k + 1), P.end(), true);
		Traits::Circle c = mc.circle();
		r_rest = c.squared_radius();
	}
	// check if k is feasible,
	if(r_k <= r_rest) {
		return max_k(P, k, b);
	} else {
		return max_k(P, a, k);
	}
}

void testcase() {
	unsigned n;
	cin >> n;

	vector<Point> P(n);

	for(unsigned i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		if(i == 0) {
			P0 = Point(x, y);
		}
		P[i] = Point(x,y);
	}

	sort(P.begin()+1, P.end(), [](Point p1, Point p2) {
		return squared_distance(P0, p1) < squared_distance(P0, p2);
	});

	unsigned k = max_k(P, 0, n);
	K::FT r_k = squared_distance(P0, P0);
	if(n > 2) {
		Min_circle mc(P.begin() + (k + 1), P.end(), true);
		Traits::Circle c = mc.circle();
		r_k = c.squared_radius();
	}
	K::FT r_k1_ = squared_distance(P0, P0);
	if(n > 2) {
		Min_circle mc(P.begin() + (k + 2), P.end(), true);
		Traits::Circle c = mc.circle();
		r_k1_ = c.squared_radius();
	}
	K::FT r_k_ = squared_distance(P0, P[k]);
	K::FT r_k1 = squared_distance(P0, P[k+1]);
	K::FT r = min(r_k, r_k1);

//	cout << r_k << " " << r_k_ << " " << r_k1 << " " << r_k1_ << endl;
	cout << r << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	cout << fixed << setprecision(0);
	while(t--) { testcase(); }
	return 0;
}
