// for algolab, by taubnert@ethz.ch
#include <climits>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

void testcase() {
	unsigned n;
	cin >> n;

	vector<int> a(n), b(n);

	for (unsigned i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (unsigned i = 0; i < n; i++) {
		cin >> b[i];
	}

	vector <vector<int>> DP(n+1);
	for (unsigned i = 0; i <= n; i++) {
		DP[i] = vector<int>(n+1);
	}
	for (unsigned i = 1; i <= n; i++) {
		for (unsigned j = 1; j <= n; j++) {
			int min_cost = INT_MAX;
			if (i == 0 && j == 0) {
				min_cost = 0;
			}
			for (unsigned i_ = 0; i_ < i; i_++) {
				for (unsigned j_ = 0; j_ < j; j_++) {
					// Make sure, not one stack is empty at the end (without the other being empty)
					if ((i_ == 0 && j_ == 0) || (i_ != 0 && j_ != 0)) {
						int Sa = accumulate(a.begin() + i_, a.begin() + i, 0);
						int Sb = accumulate(b.begin() + j_, b.begin() + j, 0);
						int cost = DP[i_][j_] + (Sa - (i - i_)) * (Sb - (j - j_));
						min_cost = min(min_cost, cost);
					}
				}
			}
			DP[i][j] = min_cost;
		}
	}

	cout << DP[n][n] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
