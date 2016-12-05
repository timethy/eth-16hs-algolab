// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void testcase() {
	unsigned n, k;
	cin >> n >> k;

	vector<long> v(n);
	vector<long> vs(n);
	for (unsigned i = 0; i < n; i++) {
		long v_i;
		cin >> v_i;
		v[i] = v_i;
		vs[i] = v_i + ((i==0) ? 0 : vs[i-1]);
	}

	vector<long> diff(n);
	vector<unsigned> j(n);
	long min_diff = INT64_MAX;
	for (long i = -1; i < n-1; i++) {
		auto bias = i == -1 ? 0 : vs[i];
		auto l = lower_bound(vs.begin()+i, vs.end(), k+bias);
		auto u = upper_bound(vs.begin()+i, vs.end(), k+bias);
		if (u == vs.end() || (l != vs.end() && k - *l <= *u - k)) {
			diff[i+1] = k - (*l-bias);
			j[i+1] = l - vs.begin();
		} else {
			diff[i+1] = (*u-bias) - k;
			j[i+1] = u - vs.begin();
		}
		min_diff = min(min_diff, abs(diff[i+1]));
	}
	unsigned i_, j_;
	for (unsigned i = 0; i < n; i++) {
		int self_diff = abs(v[i] - k);
		if (abs(self_diff) <= min_diff) {
			i_ = i;
			j_ = i;
			min_diff = self_diff;
			break;
		} else if (min_diff == diff[i]) {
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
	while(t--) { testcase(); }
	return 0;
}
