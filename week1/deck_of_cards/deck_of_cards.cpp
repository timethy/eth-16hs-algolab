// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// WA for testset 4!
void testcase() {
	long long n, k;
	cin >> n >> k;

	vector<long long> v(n);
	vector<long long> vs(n);
	for (int i = 0; i < n; i++) {
		long long v_i;
		cin >> v_i;
		v[i] = v_i;
		vs[i] = v_i + ((i == 0) ? 0L : vs[i - 1]);
	}

	vector<long long> diff(n);
	vector<int> j(n);
	long long min_diff = INT64_MAX;
	for (int i = 0; i < n; i++) {
		auto bias = vs[i] - v[i];
		auto l = lower_bound(vs.begin() + i, vs.end(), k + bias);
		auto u = upper_bound(vs.begin() + i, vs.end(), k + bias);
//		cout << i << " " << (l - vs.begin()) << " " << (u - vs.begin()) << " " << vs[i] << " " << k << endl;
		if (l == vs.end()) {
			// If all values in vs are smaller than k+bias
			j[i] = n - 1;
		} else {
			if (u == vs.end()) {
				j[i] = l - vs.begin();
			} else { // both l and u exist
				if (abs(k + bias - *l) <= abs((k + bias) - *u)) {
					j[i] = l - vs.begin();
				} else {
					j[i] = u - vs.begin();
				}
			}
		}
		diff[i] = abs(k + bias - vs[j[i]]);
		min_diff = min(min_diff, diff[i]);
	}
	int i_, j_;
	for (int i = 0; i < n; i++) {
		if (min_diff == diff[i]) {
			i_ = i;
			j_ = j[i];
			break;
		}
	}
	cout << i_ << " " << j_ << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
