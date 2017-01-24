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

	vector<int> Sa(n), Sb(n);

	int a;
	cin >> a;
	Sa[0] = a;
	for (unsigned i = 1; i < n; i++) {
		cin >> a;
		Sa[i] = Sa[i - 1] + a;
	}
	int b;
	cin >> b;
	Sb[0] = b;
	for (unsigned i = 1; i < n; i++) {
		cin >> b;
		Sb[i] = Sb[i - 1] + b;
	}

	vector <vector<int>> DP(n + 1, vector<int>(n+1));
	for (unsigned i = 1; i <= n; i++) {
		for (unsigned j = 1; j <= n; j++) {
			int min_cost = INT_MAX;
			{
				unsigned j_ = j - 1;
				int Sb_ = Sb[j - 1];
				if (j_ > 0) { Sb_ -= Sb[j_ - 1]; }
				int fb = Sb_ - (j - j_);
				for (unsigned i_ = 0; i_ < i; i_++) {
					// Make sure, not one stack is empty at the end (without the other being empty)
					if ((i_ == 0 && j_ == 0) || (i_ != 0 && j_ != 0)) {
						int Sa_ = Sa[i - 1];
						if (i_ > 0) { Sa_ -= Sa[i_ - 1]; }
						int cost = DP[i_][j_] + (Sa_ - (i - i_))*fb;
						min_cost = min(min_cost, cost);
					}
				}
			}
			{
				unsigned i_ = i - 1;
				int Sa_ = Sa[i - 1];
				if (i_ > 0) { Sa_ -= Sa[i_ - 1]; }
				int fa = Sa_ - (i - i_);
				for (unsigned j_ = 0; j_ < j; j_++) {
					// Make sure, not one stack is empty at the end (without the other being empty)
					if ((i_ == 0 && j_ == 0) || (i_ != 0 && j_ != 0)) {
						int Sb_ = Sb[j - 1];
						if (j_ > 0) { Sb_ -= Sb[j_ - 1]; }
						int cost = DP[i_][j_] + fa*(Sb_ - (j - j_));
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
