// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int EDF(vector<vector<int>>& jedis, int a_min, int b_max) {
	int a = a_min;
	int n = 0;
	for(auto it = jedis.begin(); it != jedis.end(); ++it) {
		if((*it)[1] > a && (*it)[2] < b_max) {
			// Schedule
			a = (*it)[2];
			n += 1;
		}
	}
	return n;
}

void testcase() {
	unsigned n, m;
	cin >> n >> m;

	vector<vector<int>> jedi(n);

	for(unsigned i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		jedi[i].push_back(i);
		// 0-based indexing
		jedi[i].push_back(a-1);
		jedi[i].push_back(b-1);
	}

	vector<vector<int>> segments;
	for(unsigned i = 0; i < n; i++) {
		int a = jedi[i][1], b = jedi[i][2];
		if(a <= b) {
			segments.push_back(jedi[i]);
		} else {
			vector<int> start(3), end(3);
			start[0] = end[0] = i;
			start[1] = a;
			start[2] = m-1;
			end[1] = 0;
			end[2] = b;
			segments.push_back(start);
			segments.push_back(end);
		}
	}
	// Sort by a.
	sort(segments.begin(), segments.end(), [](vector<int> j1, vector<int> j2) {
		return j1[1] < j2[1];
	});

	// Now find segment i with at most 10 overlap (for now 0)
	int o = 0;
	int b = 0;
	for(auto it = segments.begin(); it != segments.end(); ++it) {
		// If next a is after current b
		if(b < (*it)[1]) {
			o = (*it)[1]-1;
			break;
		} else {
			b = max(b, (*it)[2]);
		}
	}

//	cout << o << endl;

	// Re-center (no split for now)
	for(unsigned i = 0; i < n; i++) {
		int a = jedi[i][1], b = jedi[i][2];
		if(a > o) {
			jedi[i][1] = a-o;
		} else {
			jedi[i][1] = a-o+m;
		}
		if(b > o) {
			jedi[i][2] = b-o;
		} else {
			jedi[i][2] = b-o+m;
		}
/*		cout << jedi[i][0] << endl;
		cout << jedi[i][1] << endl;
		cout << jedi[i][2] << endl;*/
	}

	vector<vector<int>> no_overlap = jedi;
	// sort by b ("deadline")
	sort(no_overlap.begin(), no_overlap.end(), [](vector<int> j1, vector<int> j2) {
		return j1[2] < j2[2];
	});

	cout << EDF(no_overlap, 0, m+1) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
