#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <bitset>
using namespace std;

const int W = 17;

void testcases() {
	int h, w;
	cin >> h >> w;

	vector<bitset<W>> board(h);

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			char b;
			cin >> b;
			if(i >= 1 && i < h-1) {
				if(j >= 1 && j < w-1 && b == '0') {
					board[i-1].set(j-1);
				}
			}
		}
	}
	h = h-2;
	w = w-2;
	if(h <= 1 || w <= 1) {
		cout << 0 << endl;
		return;
	}

	vector<vector<int>> DP(h,vector<int>(1<<w));
	auto ALL = bitset<W>((1<<w)-1);

	for(int i = 1; i < h; i++) {
		for(int num_blocked = w; num_blocked >= 0; num_blocked--) {
			for(int s = 0; s < (1<<w); s++) {
				auto S = bitset<W>(s);
				// check if S is feasible:
				if(S.count() == num_blocked) {
					bool feasible = true;//(S & board[i]).none() && (S & board[i-1]).none();
					int max_poss = 0;
					int num_tiles = 0;
					if(feasible) {
						for(int j = 0; j < w-1;) {
							if(!S.test(j) && !S.test(j+1) && !board[i][j] && !board[i][j+1] && !board[i-1][j] && !board[i-1][j+1]) {
								num_tiles += 1;
								//cout << i << "," << j << " in " << S << "," << board[i] << "," << board[i-1] << endl;
								j += 2;
							} else {
								j++;
							}
						}
						auto S_flipped = ALL xor S;
						max_poss = num_tiles + DP[i-1][S_flipped.to_ulong()];
						//cout << i << "," << S << " | " << num_tiles << "+" << DP[i-1][S_flipped.to_ulong()] << endl;
					}
					for(int j = 0; j < w; j++) {
						auto S_ = S;
						// if j is free check what we would have got if j is blocked instead
						if(!S.test(j)) {
							S_.flip(j);
							max_poss = max(max_poss, DP[i][S_.to_ulong()]);
						}
					}
					DP[i][S.to_ulong()] = max_poss;
				//	cout << i << "," << S << " | " << max_poss << endl;
				}
			}
		}
	}/*
	for(int i = 0; i < h; i++) {
		cout << DP[i][0] << endl;
	}*/

	cout << DP[h-1][0] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--)	testcases();
	return 0;
}
