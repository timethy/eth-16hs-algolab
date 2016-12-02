#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int max_priority(vector<int>& potentials, unsigned int l) {
	sort(potentials.rbegin(), potentials.rend());
	int max = 0;
//	cout << "###";
	for(vector<int>::iterator it = potentials.begin(); it != potentials.end(); ++it) {
//	cout << *it << " ";
		if(distance(potentials.begin(), it) == l) {
			break;
		}
		max += *it;
	}
//	cout << "\n";
	return max;
}

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	cin >> t;
	for (; t > 0; t--) {
		int n, m, l;
		cin >> n >> m >> l;

		vector<int> DP(n);
		// stores for every stop: start -> priority
		vector<multimap<int, int>> missions(n);

		for (int i = 0; i < m; i++) {
			int x, y, q;
			cin >> x >> y >> q;
			missions[y].insert(make_pair(x, q));
		}

		DP[0] = 0;
		for (int i = 1; i < n; i++) {
			int max_ = 0;
			for (int j = i-1; j >= 0; j--) {
				vector<int> potentials;
				for (int p = j+1; p <= i; p++) {
					pair<map<int, int>::iterator, map<int,int>::iterator> iters = missions[p].equal_range(j);
					for(map<int, int>::iterator it = iters.first; it != missions[p].end(); ++it) {
//						cout << "x = " << it->first << ", q = " << it->second << "\n";
						potentials.push_back(it->second);
					}
				}
				max_ = max(max_, DP[j] + max_priority(potentials, l));
//				cout << "i= " << i << ", j= " << j << ", pot.size()= " << potentials.size() << ", max= " << max_ << "\n";
			}
			DP[i] = max_;
//			cout << DP[i] << "\n";
		}

		cout << DP[n-1] << "\n";
	}
}

