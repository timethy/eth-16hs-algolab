// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef  K::Point_2 Point;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

using namespace std;
Point P0;

void testcase() {
	unsigned n;
	cin >> n;

	vector<Point> P(n);

	// Read in all points, P0 is main city (= centre of transmitting station 1)
	for(unsigned i = 0; i < n; i++) {
		int x, y;
		cin >> P[i];
		if(i == 0) {
			P0 = P[i];
		}
	}

	// Sort all but the main city according to squared_distance
	sort(P.begin()+1, P.end(), [](Point p1, Point p2) {
		return squared_distance(P0, p1) < squared_distance(P0, p2);
	});

	// Now build up min_circle one-by-one until its one bigger:
	int k;
	auto r_rest = squared_distance(P[n-1],P[n-1]);
	Min_circle mc;
	for(k = n-1; k >= 0; k--) {
		r_rest = mc.circle().squared_radius();
//		cout << r_rest << " vs. " << squared_distance(P0, P[k]) << endl;
		if(r_rest > squared_distance(P0, P[k])) {
			break;
		}
		mc.insert(P[k]);
	}

	cout << ceil(min(squared_distance(P0, P[k + 1]), r_rest)) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	cout << fixed << setprecision(0);
	while(t--) { testcase(); }
	return 0;
}
