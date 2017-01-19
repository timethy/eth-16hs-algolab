// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int schedule(vector<vector<int>> &jedi, vector<int> &overlapping, int a_min, int b_max) {
	int a = a_min;
	int count = 0;
	for(auto it = jedi.begin(); it != jedi.end(); ++it) {
		vector<int> j = *it;
		auto overlaps = find(overlapping.begin(), overlapping.end(), j[0]);
		if(overlaps == overlapping.end()) {
			if(a <= j[1] && j[2] < b_max) {
				a = j[2]+1;
				count += 1;
			}
		}
	}
	return count;
}

void testcase() {
	unsigned n, m;
	cin >> n >> m;

	vector<vector<int>> jedi(n);
	multimap<int, int> jedi_a;

	for(unsigned i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		jedi[i].push_back(i);
		jedi[i].push_back(a-1);
		jedi[i].push_back(b-1);
//		jedi_a.insert(make_pair(a, i));
//		jedi_a.insert(make_pair(b, i));
	}

	int i_10 = 0;
/*	for(unsigned i = 0; i < n; i++) {
		auto it_start = jedi_a.lower_bound(jedi[i][1]);
		auto it_end = jedi_a.upper_bound(jedi[i][2]);
		auto it = it_start;
		int count = 0;
		while(it != it_end && count <= 20) {
			it++;
			count += 1;
		}
		if(count <= 20) {
			i_10 = i;
			break;
		}
	}*/

	int a0 = jedi[i_10][1];
	for(unsigned i = 0; i < n; i++) {
		cout << a0 << " " << jedi[i][1] << " " << jedi[i][2] << endl;
		jedi[i][1] = (m + jedi[i][1] - a0) % m;
		jedi[i][2] = (m + jedi[i][2] - a0) % m;
		cout << a0 << " " << jedi[i][1] << " " << jedi[i][2] << endl;
	}
	auto it_start = jedi_a.lower_bound(jedi[i_10][1]);
	auto it_end = jedi_a.upper_bound(jedi[i_10][2]);
	vector<int> overlapping;
	for(auto it = it_start; it != it_end; it++) {
		overlapping.push_back(it->second);
	}

	sort(jedi.begin(), jedi.end(), [](vector<int> j1, vector<int> j2) {
		return j1[2] < j2[2];
	});

	int c = schedule(jedi, overlapping, 0, m);
	for(auto it = overlapping.begin(); it != overlapping.end(); it++) {
		vector<int> j = jedi[*it];
		c = max(schedule(jedi, overlapping, j[1], j[2])+1,c);
	}

	cout << c << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
