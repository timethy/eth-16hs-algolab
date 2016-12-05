// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <bitset>
#include <set>
#include <algorithm>

using namespace std;

// RECALL from DnA
void testcase() {
	unsigned n, k;
	cin >> n >> k;

	vector<char> outcomes(k);
	vector<vector<int>> lefts(k);
	vector<vector<int>> rights(k);

	// 111 = all poss,
	// 011 = < or equal
	// 101 = > or equal
	// 001 = correct,
	int EQ = 0;
	int LT = 1;
	int GT = 2;
	vector<bitset<3>> coins;

	for(unsigned i = 0; i < k; i++) {
		unsigned P_i;
		cin >> P_i;
		vector<int> &left = lefts[i];
		vector<int> &right = rights[i];
		for (unsigned j = 0; j < P_i; j++) {
			int coin;
			cin >> coin;
			left.push_back(coin);
		}
		for (unsigned j = 0; j < P_i; j++) {
			int coin;
			cin >> coin;
			right.push_back(coin);
		}
		cin >> outcomes[i];
	}

	int c = 0;
	for(unsigned i = 0; i < k; i++) {
		char outcome = outcomes[i];
		vector<int> &left = lefts[i];
		vector<int> &right = rights[i];
		if(outcome == '<') {
			auto tmp = left;
			left = right;
			right = left;
		}
		if(outcome == '=') {
			for(set<int>::iterator it = left.begin(); it != left.end(); it++) {
				coins[*it-1].reset(GT);
				coins[*it-1].reset(LT);
			}
			for(set<int>::iterator it = right.begin(); it != right.end(); it++) {
				coins[*it-1].reset(GT);
				coins[*it-1].reset(LT);
			}
		} else {
			for(set<int>::iterator it = left.begin(); it != left.end(); it++) {
				coins[*it-1].reset(GT);
			}
			for(set<int>::iterator it = right.begin(); it != right.end(); it++) {
				coins[*it-1].reset(LT);
			}
		}
	}

	// Now either one coin is heavier or lighter then the rest of coins
	int c = -1;
	for(vector<int>::iterator it = coins.begin(); it != coins.end(); it++) {

	}
	return c;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
