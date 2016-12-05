// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <bitset>
#include <set>
#include <algorithm>

using namespace std;

// IDEA:
// IFF there is exactly one coin that has a different weight its either lighter or heavier.
// IF besaid coin is lighter, it must appear in ALL '<' measurements on the left and none in the right (vice-versa for '>')
//    and none '=' measurements.
// IF it is heavier, than vice-versa
void testcase() {
	unsigned n, k;
	cin >> n >> k;

	int IN_total = 0, E_total = 0;
	// (coin#id - 1) -> number of L, H, E measurements
	vector<int> L(n, 0), H(n, 0), E(n, 0);

	for (unsigned i = 0; i < k; i++) {
		unsigned P_i;
		char outcome;
		cin >> P_i;
		vector<int> left, right;
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
		cin >> outcome;
		if (outcome == '=') {
			E_total += 1;
			for (auto it = left.begin(); it != left.end(); it++) {
				E[*it - 1] += 1;
			}
			for (auto it = right.begin(); it != right.end(); it++) {
				E[*it - 1] += 1;
			}
		} else if (outcome == '<') {
			IN_total += 1;
			for (auto it = left.begin(); it != left.end(); it++) {
				L[*it - 1] += 1;
			}
			for (auto it = right.begin(); it != right.end(); it++) {
				H[*it - 1] += 1;
			}
		} else {
			IN_total += 1;
			for (auto it = left.begin(); it != left.end(); it++) {
				H[*it - 1] += 1;
			}
			for (auto it = right.begin(); it != right.end(); it++) {
				L[*it - 1] += 1;
			}
		}
	}

	int coin = 0;
	int count = 0;
	for (unsigned i = 1; i <= n; i++) {
		int lt = L[i - 1], gt = H[i - 1], eq = E[i - 1];
		if (eq == 0 && lt == IN_total && gt == 0) {
			count += 1;
			coin = i;
		} else if (eq == 0 && gt == IN_total && lt == 0) {
			count += 1;
			coin = i;
		}
	}
	if (count == 1) {
		cout << coin << endl;
	} else {
		cout << 0 << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
