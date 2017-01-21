// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

inline int EDF(vector<pair<int, int>>& jedis, int a_min, int b_max) {
	int a = a_min;
	int n = 0;
	for(auto it = jedis.begin(); it != jedis.end(); ++it) {
		if(it->first > a && it->second < b_max) {
			// Schedule
			a = it->second;
			n += 1;
		}
	}
	return n;
}

void testcase() {
	int n, m;
	cin >> n >> m;

	vector<pair<int, int>> jedi(n);

	map<int, int> counter;

	for(int i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		a -= 1;
		b -= 1;
		// 0-based indexing
		jedi[i].first = a;
		jedi[i].second = b;

		counter[a]++;
		counter[b]--;
		if(a > b) {
			counter[0]++;
		}
	}

	// Now find segment i with at most 10 overlap (for now 0)
	int o = 0;
	int min_k = n;
	int k = 0;
	for(auto it = counter.begin(); it != counter.end(); ++it) {
		k += it->second;
		if(k <= 10 && k < min_k) {
			o = it->first;
			min_k = k;
		}
	}

//	cout << o << " " << min_k << endl;

	// Re-center and split
	vector<pair<int, int>> overlap;
	vector<pair<int, int>> no_overlap;
	for(int i = 0; i < n; i++) {
		int a = jedi[i].first, b = jedi[i].second;
		// recenter
		if(a > o) {
			jedi[i].first = a-o;
		} else {
			jedi[i].first = a-o+m;
		}
		if(b > o) {
			jedi[i].second = b-o;
		} else {
			jedi[i].second = b-o+m;
		}
		// These here are still old a's and b's:
		if((a <= b && a <= o && o <= b) ||
				(a > b && (a <= o || o <= b))) {
			// overlaps
			overlap.push_back(jedi[i]);
		} else {
			no_overlap.push_back(jedi[i]);
		}
	}

//	cout << overlap.size() << "+" << no_overlap.size() << endl;

	// sort by b ("deadline")
	sort(no_overlap.begin(), no_overlap.end(), [](pair<int, int> j1, pair<int, int> j2) {
		return j1.second < j2.second;
	});

	int n_max = EDF(no_overlap, -1, m);
	for(auto it = overlap.begin(); it != overlap.end(); it++) {
//		cout << it->first << it->second << endl;
		n_max = max(n_max, EDF(no_overlap, it->second % m, it->first)+1);
	}
	cout << n_max << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
