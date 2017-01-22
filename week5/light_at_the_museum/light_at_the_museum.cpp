// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <bitset>
#include <climits>
#include <map>
#include <algorithm>

using namespace std;

typedef vector<vector<int>> vvec;

vector<int> works(vector<int>& a, vvec& l_diff, bitset<32> S) {
	vector<int> b = a;
	for(unsigned i = 0; i < l_diff.size(); i++) {
		if(S.test(i)) {
			for(unsigned j = 0; j < a.size(); j++) {
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

	// All 2^N subsets
	size_t min_count = INT_MAX;
	for(int s = 0; s < (1<<N); s++) {
		auto S = bitset<32>(s);
		if(works(a, l_diff, S) == b) {
			min_count = min(min_count, S.count());
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
