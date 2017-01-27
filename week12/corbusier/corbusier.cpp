#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

typedef vector<vector<bool>> vvec;

void testcase() {
	int n, i, k;
	cin >> n >> i >> k;

	//cout << n << " " << i << " " << k;

	vector<int> h_mod_k(n);
	for(int a = 0; a < n; a++) {
		long h;
		cin >> h;
		h_mod_k[a] = h % k;
	}

	vvec DP(n, vector<bool>(k, false));

	DP[0][0] = true;
	DP[0][h_mod_k[0]] = true;

	for(int a = 1; a < n; a++) {
		for(int b = 0; b < k; b++) {
			int i_take = (k+b-h_mod_k[a]) % k;
			DP[a][b] = DP[a-1][i_take] || DP[a-1][b];
		}
	}

	/*
	for(int a = 0; a < n; a++) {
		for(int b = 0; b < k; b++) {
			cout << DP[a][b];
		}
		cout << endl;
	}*/

	// Backtrack solution and check for non-emptiness

	bool non_empty_solution = false;
	int a = n-1;
	int b = i;
	while(DP[a][b] && a > 0) {
		int i_take = (k+b-h_mod_k[a]) % k;
		if(DP[a-1][i_take]) {
			non_empty_solution = true;
			b = i_take;
		} else {
			b = i;
		}
		a -= 1;
	}

	cout << (non_empty_solution ? "yes" : "no") << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--) testcase();
	return 0;
}
