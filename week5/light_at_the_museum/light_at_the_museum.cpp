// for algolab, by taubnert@ethz.ch
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <algorithm>

using namespace std;

typedef vector<vector<int>> vvec;

inline vector<int> diff(unsigned M, vvec& l_diff, bitset<30> S, int offset) {
	vector<int> b(M);
	for(unsigned i = offset; i < l_diff.size(); i++) {
		if(S.test(i-offset)) {
			for(unsigned j = 0; j < M; j++) {
				b[j] += l_diff[i][j];
			}
		}
	}
	return b;
}

void testcase() {
	unsigned N, M;
	cin >> N >> M;
	vvec l_on(N, vector<int>(M));
	vvec l_off(N, vector<int>(M));
	vvec l_diff(N, vector<int>(M));

	vector<int> a(M);
	vector<int> b(M);
	for(unsigned i = 0; i < M; i++) {
		cin >> b[i];
	}
	for(unsigned i = 0; i < N; i++) {
		for(unsigned j = 0; j < M; j++) {
			cin >> l_on[i][j] >> l_off[i][j];
			l_diff[i][j] = l_off[i][j] - l_on[i][j];
			a[j] += l_on[i][j];
		}
	}

	unsigned N1 = N/2, N2 = N-N1;
//	cout << N1 << " " << N2 << endl;

	// Precompute b_S2
	vector<pair<vector<int>, int>> d2_S2(1<<N2);
	for(int s = 0; s < 1<<N2; s++) {
		auto S2 = bitset<30>(s);
		d2_S2[s] = make_pair(diff(M, l_diff, S2, N1), s);
	}

	// Sort lexicographically
	sort(d2_S2.begin(), d2_S2.end(), [](pair<vector<int>, int> x1, pair<vector<int>, int> x2) {
		if(x1.first == x2.first) {
			return bitset<30>(x1.second).count() < bitset<30>(x2.second).count();
		} else {
			return x1.first < x2.first;
		}
	});
	vvec d2;
	for(auto it=d2_S2.begin(); it != d2_S2.end(); it++) {
		d2.push_back(it->first);
	}

	size_t min_count = INT_MAX;
	// All 2^N/2 subsets S_1
	for(int s = 0; s < 1<<N1; s++) {
		auto S1 = bitset<30>(s);
		auto d1 = diff(M, l_diff, S1, 0);
		auto d2_ = vector<int>(M);
		for(unsigned j = 0; j < M; j++) {
			d2_[j] = b[j] - a[j] - d1[j];
//			cout << "b=" << b[j] << ", a=" << a[j] << ", d1=" << d1[j] << ", d2=" << d2_[j] << endl;
		}
		if(binary_search(d2.begin(), d2.end(), d2_)) {
			auto it = lower_bound(d2.begin(), d2.end(), d2_);
			assert(*it == d2_);
			int idx = it - d2.begin();
			auto S2 = bitset<30>(d2_S2[idx].second);
			min_count = min(min_count, S1.count() + S2.count());
//			cout << S1 << " " << S2 << endl;
//			cout << min_count << endl;
		}
	}

	if(min_count == INT_MAX) {
		cout << "impossible" << endl;
	} else {
		cout << min_count << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
