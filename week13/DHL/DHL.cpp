// for algolab, by taubnert@ethz.ch
#include <climits>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

typedef vector <vector<int>> vvec;

vvec MM;

int memo(vector<int> &Sa, vector<int> &Sb, unsigned i, unsigned j) {
	if (i == 0 || j == 0) {
		if (i == 0 && j == 0) {
			return 0;
		} else {
			return INT_MAX/2;
		}
	} else if (MM[i][j] >= 0) {
		return MM[i][j];
	} else {
		// Take one from left
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
					int cost = memo(Sa, Sb, i_, j_) + (Sa_ - (i - i_))*fb;
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
					int cost = memo(Sa, Sb, i_, j_) + fa*(Sb_ - (j - j_));
					min_cost = min(min_cost, cost);
				}
			}
		}
		MM[i][j] = min_cost;
		return min_cost;
	}
}

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

	MM = vvec(n + 1, vector<int>(n + 1, -1));
	cout << memo(Sa, Sb, n, n) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
