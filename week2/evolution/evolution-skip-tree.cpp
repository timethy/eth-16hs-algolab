#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Node {
public:
	bool operator< (const Node& other) {
	// ascending ages
		return age <= other.age;
	}
	string name;
	int age;
	unsigned int lvl; // ROOT has lvl 0, 1, 2, 3, ...
	unsigned int idx;
	int ancestor;
	// skiplist to 1st, 2nd, 4th ancestor and so on
	vector<int> skiplist;
};

int findBiggestSmaller(vector<Node> &species, int s, int a) {
	vector<int> skiplist = species[s].skiplist;
	if(skiplist.size() == 0) {
		return s; // If we are root
	} else if(skiplist.size() == 1) {
		if (species[skiplist.front()].age > a) {
			return s;
		} else {
			return findBiggestSmaller(species, skiplist.front(), a);
		}
	} else {
		// find two ancestors on skip list smaller and bigger
		vector<int>::iterator l = skiplist.begin();
		vector<int>::iterator r = l;
		while(r != skiplist.end() && species[*r].age <= a) {
			l = r;
			r = r + 1;
		}
		if (l == r) {
			return s;
		}
		return findBiggestSmaller(species, *l, a);
	}
}

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	int i, j;
	cin >> t;
	for (i = 0; i < t; i++) {
		vector<Node> species;
		map<string, string> ancestors;

		int n, q;
		cin >> n >> q;

		for (j = 0; j < n; j++) {
			Node n;
			cin >> n.name >> n.age;
			n.ancestor = -1;
			n.lvl = 0;
			species.push_back(n);
		}
		// sort species by age (ascending)
		sort(species.begin(), species.end());

		// read in ancestor-relation
		for (j = 0; j < n-1; j++) {
			string s, p;
			cin >> s >> p;
			ancestors[s] = p;
		}
		// build name-idx, set idx
		map<string,int> byname;
		for (vector<Node>::iterator it(species.begin());
		     it != species.end(); it++) {
			it->idx = it - species.begin();
			byname[it->name] = it->idx;
		}
		// get levels, build skip-list
		for (vector<Node>::reverse_iterator it(species.rbegin());
		     it != species.rend(); it++) {
			// Set ancestors if it's not the root
			if (ancestors.find(it->name) != ancestors.end()) {
				it->ancestor = byname[ancestors[it->name]];
				it->lvl = species[it->ancestor].lvl + 1;
				it->skiplist.push_back(it->ancestor);
				int i = it->lvl;
				while(i != 0 && i % 2 == 0) {
					it->skiplist.push_back(species[it->skiplist.back()].skiplist.back());
					i /= 2;
				}
			} // else nothing
		}
		for (j = 0; j < q; j++) {
			string s;
			int b;
			cin >> s >> b;
			int s0 = byname[s];
			int si = findBiggestSmaller(species, s0, b);
			cout << species[si].name;
			if (j != q - 1) { cout << " "; }
		}
		cout << "\n";
	}
}