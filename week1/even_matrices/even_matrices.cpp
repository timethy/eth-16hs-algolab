// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

// TODO: BGL
// TODO: CGAL

using namespace std;

void testcase() {
	unsigned n;
	cin >> n;

	vector<vector<int>> X(n, vector<int>(n));
	vector<vector<int>> sums(n, vector<int>(n));
	// TODO Why is it so slow with maps?
	//map<int, map<int, int>> sums;
	int n_pairs = 0;

	for (unsigned i = 0; i < n; i++) {
		for (unsigned j = 0; j < n; j++) {
			cin >> X[i][j];
		}
	}

	for (unsigned j = 0; j < n; j++) {
		int sum = 0;
		for (unsigned i = 0; i < n; ++i) {
			sum += X[i][j];
			sums[j][i] = sum;
		}
	}

	// Just sum up all possiblities
	for (unsigned i_start = 0; i_start < n; ++i_start) {
		for (unsigned i_end = i_start; i_end < n; ++i_end) {
			int even = 0;
			int odd = 0;
			for (unsigned j = 0; j < n; j++) {
				int sum = sums[j][i_end] - (i_start == 0 ? 0 : sums[j][i_start-1]);
				if(j == 0) {
					if(sum % 2 == 0) { even = 1; }
					else { odd = 1; }
				} else {
					if(sum % 2 == 0) {
						even += 1;
					} else {
						int tmp = even;
						even = odd;
						odd = tmp+1;
					}
				}
				n_pairs += even;
			}
		}
	}

	cout << n_pairs << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
