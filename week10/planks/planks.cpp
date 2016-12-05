// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

int combinations(int a, vector<unsigned> &ls, bitset<20> used, int edge) {
	if(edge == 4) {
		return 1;
	} else {
		// all subsets:
		int l_count = ls.size();
		int feasible = 0;
		for (int i = 0; i < (1 << l_count); i++) {
			bitset<20> new_edges(i);
			if ((used & new_edges).none()) {
				// return 0 if infeasible, 1 if feasible
				int b = 0;
				for (unsigned i = 0; i < ls.size(); i++) {
					if (new_edges[i]) {
						b += ls[i];
					}
				}
				if (a == b) {
//					cout << edge << " " << used << " " << new_edges << endl;
					feasible += combinations(a, ls, used | new_edges, edge + 1);
				}
			}
		}
		return feasible;
	}
}

void testcase() {
	unsigned n;
	cin >> n;

	unsigned l_total = 0;
	vector<unsigned> ls(n);
	for (unsigned i = 0; i < n; i++) {
		cin >> ls[i];
		l_total += ls[i];
	}

	// square_size must be l_total/4
	int a = l_total / 4;
	int c = combinations(a, ls, bitset<20>(0), 0);

	if(c > 0) {
		cout << 1 << endl;
	} else {
		cout << 0 << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
