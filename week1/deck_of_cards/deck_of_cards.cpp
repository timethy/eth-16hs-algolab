// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

// WA for testset 4!
void testcase() {
	int n, k;
	cin >> n >> k;

	// IDEA: Build vs[k] = sum_i=1^k v[i];
	// IDEA: Loop through 1..n and search (binary) for vs[k]+k in vs[k..n]

	vector<int> v(n);
	vector<int> vs(n);
	for (int i = 0; i < n; i++) {
		int v_i;
		cin >> v_i;
		v[i] = v_i;
		vs[i] = v_i + ((i == 0) ? 0 : vs[i - 1]);
	}

	int a, b;
	int min_diff = INT_MAX;
	for (int i = 0; i < n; i++) {
		auto lb = lower_bound(vs.begin() + i, vs.end(), k + vs[i] - v[i]);
		auto ub = upper_bound(vs.begin() + i, vs.end(), k + vs[i] - v[i]);
		int lower = INT_MAX;
		int equal = INT_MAX;
		int upper = INT_MAX;
		if (lb != vs.begin() + i) {
			lower = *(lb-1) - vs[i] + v[i];
		}
		if (lb != vs.end()) {
			equal = *lb - vs[i] + v[i];
		}
		if (ub != vs.end()) {
			upper = *ub - vs[i] + v[i];
		}
//		cout << lower << " " << upper << endl;
		if(abs(lower-k) < min_diff) {
			a = i;
			b = (lb-vs.begin())-1;
			min_diff = abs(lower-k);
		}
		if(abs(equal-k) < min_diff) {
			a = i;
			b = lb-vs.begin();
			min_diff = abs(equal-k);
		}
		if(abs(upper-k) < min_diff) {
			a = i;
			b = ub-vs.begin();
			min_diff = abs(upper-k);
		}
	}

	cout << a << " " << b << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
