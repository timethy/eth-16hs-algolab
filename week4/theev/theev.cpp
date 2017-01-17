// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

using namespace std;

void testcase() {
	unsigned n;
	cin >> n;

	vector<K::Point_2> P(n);
	K::Point_2 P0;

	for(unsigned i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		if(i == 0) {
			P0 = K::Point_2(x, y);
		}
		P[i] = CGAL::ORIGIN + (K::Point_2(x, y) - P0);
	}

	sort(P.begin(), P.end(), [](K::Point_2 p1, K::Point_2 p2) {
		K::Compute_squared_distance_2 squared_distance;
		return p1.x()*p1.x()+p1.y()*p1.y() < p2.x()*p2.x()+p2.y()*p2.y();
	});

	int a = 0;   // first infeasible
	int b = n-1; // first feasible

	while(true) {
//		cout << a << " " << b << endl;
		int k = (a + b)/2;
		auto r1 = P[k].x()*P[k].x()+P[k].y()*P[k].y();

		// Min_circle through points k+1..n
		Min_circle mc(P.begin()+k+1, P.end(), true);
		Traits::Circle c = mc.circle();
		auto r2 = c.squared_radius();
		auto r0 = r2;
		r0 = 0;
		if(a + 1 == b) { // last checked was a
			cout << min(r2,P[b].x()*P[b].x()+P[b].y()*P[b].y()) << endl;
			break;
		}
		// Feasible iff r2 <= r1
		if(r2 <= r1) {
			b = k;
		} else {
			a = k;
		}
	}
	// Now in b is the min. value that is still feasible
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
