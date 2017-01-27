#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

typedef vector<vector<pair<int, int>>> vvec;

void testcase() {
	int n, k;
	cin >> n >> k;

	vector<int> c(n), v(n);
	for(int i = 0; i < n; i++) {
		cin >> c[i] >> v[i];
	}

	if(k == 0) {
		cout << 0 << " " << 0 << endl;
		return;
	}

	int V_max = k + *max_element(v.begin(), v.end());

	//cout << k << " " << V_max << endl;

	vvec DP(n, vector<pair<int, int>>(V_max+1));

	DP[0][0] = make_pair(0,0);
	for(int j = 1; j <= V_max; ++j) {
		// Ceil(j/v[0])
		int n_needed = (j+v[0]-1)/v[0];
		DP[0][j] = make_pair(n_needed*c[0], 1);
	}

	for(int i = 1; i < n; i++) {
		DP[i][0] = DP[i-1][0];
		for(int j = 1; j <= V_max; ++j) {
			auto take_none = DP[i-1][j];
			int min_cost, max_num;
			if(j < v[i]) {
				min_cost = c[i];
				max_num = 1;
			} else {
				pair<int, int> take_one = DP[i][j-v[i]];
				min_cost = take_one.first + c[i];;
				max_num = take_one.second + (DP[i-1][j-v[i]] == DP[i][j-v[i]] ? 1 : 0);
			}
			if(take_none.first < min_cost) {
				min_cost = take_none.first;
				max_num = take_none.second;
			} else if(min_cost == take_none.first) {
				max_num = max(max_num, take_none.second);
			}
			DP[i][j] = make_pair(min_cost, max_num);
		}
	}

/*
for(int j = 0; j < v[i]; ++j) {
	auto take_none = DP[i-1][j];
	auto take_one = make_pair(c[i], 1);
	int min_cost = take_one.first;
	int max_num = take_one.second;
}
	for(int j = 0; j <= V_max; ++j) {
		for(int i = 0; i < n; i++) {
			cout << DP[i][j].first << "," << DP[i][j].second << " ";
		}
		cout << endl;
	}

	cout << endl;*/

	cout << DP[n-1][k].first << " " << DP[n-1][k].second << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--) testcase();
	return 0;
}
