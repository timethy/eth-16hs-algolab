// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void testcase() {
	unsigned n, m;
	cin >> n >> m;

	vector<vector<int>> jedi(n);

	for(unsigned i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		jedi[i].push_back(i);
		jedi[i].push_back(a);
		jedi[i].push_back(b);
	}
	sort(jedi.begin(), jedi.end(), [](vector<int> v1, vector<int> v2) {
		return v1[1] < v2[1];
	});
	int n_segment = 0;
	int a_ = 0, b_ = 0;
	// TODO: Overlapping is wrong
	multimap<int,int> bs;
	vector<unsigned> overlapping;
	for(unsigned i = 0; i <= n; i++) {
		unsigned j = jedi[i][0];
		int a = jedi[i][1];
		int b = jedi[i][2];
		// Delete all with b <= a
		auto end = b_inv.lower_bound(a);
		n_segment -= distance(b_inv.begin(), end);
		b_inv.erase(b_inv.begin(), end);
		b_inv.insert(make_pair(b, j));
		n_segment += 1;
		if(a_ < a && n_segment <= 10) {
			for(auto it = a_inv.begin(); it != a_inv.end(); it++) {
				overlapping.push_back(it->second);
			}
			break;
		}
		a_ = a;
	}
	for (unsigned i = 0; i < n; i++) {
		jedi[i][2] = (m - b_ + jedi[i][2]) % m;
		jedi[i][1] = (m - b_ + jedi[i][1]) % m;
	}
	sort(jedi.begin(), jedi.end(), [](vector<int> v1, vector<int> v2) {
		return v1[2] < v2[2];
	});
	int n_min = n;
	cout << overlapping.size() << endl;
	for(unsigned o = 0; o < overlapping.size(); o++) {
		int n_fight = 0;
		b_ = 0;
		for (unsigned i = 0; i < n; i++) {
			int a = jedi[i][1];
			int b = jedi[i][2];
			bool no_overlap = overlapping.end() == find(overlapping.begin(), overlapping.end(), i);
			if((i == overlapping[o]) || no_overlap) {
				if (a >= b_) {
					b_ = b;
					n_fight += 1;
				}
			}
		}
		n_min = min(n_min, n_fight);
	}

	cout << n_min << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
