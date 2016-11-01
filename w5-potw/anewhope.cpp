#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Node {
public:
	bool operator< (const Node& other) {
		return lvl >= other.lvl;
	}
	int lvl;

	vector<bool> feasible;
	vector<int> max_children;

	vector<int> successors;

	// y supervised by x
	map<int, vector<int>> super;
	map<int, vector<int>> supered;
	// key is parent. y in this commando supervised by x in other commando
	map<int, vector<pair<int, int>>> supered_parents;
};

int set_level(vector<Node> &C, Node &n) {
	int max_lvl = n.lvl;
	for(vector<int>::iterator s_i(n.successors.begin());
		s_i != n.successors.end(); s_i++) {
		C[*s_i].lvl = n.lvl + 1;
		max_lvl = max(set_level(C, C[*s_i]), max_lvl);
	}
	return max_lvl;
}

int bitcount(int j) {
	if(j > 0) {
		return bitcount(j >> 1) + (j % 2);
	}
	return 0;
}

void set_feasible(Node &C_i, int k, int i) {
	if(i < k) {
		set_feasible(C_i, k, i+1);
		for(int j = 0; j < (1 << i); j++) {
			if(C_i.feasible[j]) {
				bool for_all = true;
				for(int a = 0; a < i; a++) {
					if(((a bitand j) > 0) &&
					   (C_i.super.find(i) != C_i.super.end() or
					    C_i.supered.find(i) != C_i.supered.end())) {
						for_all = false;
					}
				}
				C_i.feasible[(1 << i) bitor j] = for_all;
			}
		}
	}
}

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	cin >> t;
	for (; t > 0; t--) {
		int k, s, m;
		cin >> k >> s >> m;

		vector<Node> C(k);
		map<int, vector<int>> super_by;
		for (vector<Node>::iterator it = C.begin(); it != C.end(); it++) {
			// make enough space
			it->feasible = vector<bool>(1 << s, 1);
			it->max_children = vector<int>(1 << s, 0);
			it->supered_parents = map<int, vector<pair<int,int>>>();
		}

		// read in supervise relation
		for (int i = 0; i < m; i++) {
			int u, v, h;
			cin >> u >> v >> h;
			for (int j = 0; j < h; j++) {
				int x, y;
				cin >> x >> y;
				if (u == v) {
					C[u].super[x].push_back(y);
					C[u].supered[y].push_back(x);
				} else {
					C[u].successors.push_back(v);
					if(C[v].supered_parents.find(u) != C[v].supered_parents.end()) {
						C[v].supered_parents[u].push_back(make_pair(y, x));
					} else {
						C[v].supered_parents.insert(make_pair(u, vector<pair<int,int>>(1, make_pair(y, x))));
					}
				}
			}
		}

		for(vector<Node>::iterator it(C.begin());
		    it != C.end(); it++) {
			// generate all feasible sets
			set_feasible(*it, s, 0);
		}
		int max_lvl = set_level(C, C[0]);
		cout << "max_lvl " << max_lvl << "\n";
		for(int z = 0; z <= max_lvl; z++) {
			for (vector<Node>::iterator it(C.begin());
		     	 it != C.end(); it++) {
			if(it->lvl == z) {
				for(int j = 0; j < (1 << s); j++) {
					if(it->feasible[j]) {
						cout << "feasible: " << j << "\n";
						// Number of troopers = bitcount of j
						for(map<int, vector<pair<int, int>>>::iterator pit = it->supered_parents.begin();
							pit != it->supered_parents.end(); pit++) {
							Node* P = &C[pit->first];
							int cover = (1 << s) - 1;
							for(vector<pair<int,int>>::iterator vit = pit->second.begin();
								vit != pit->second.end(); vit++) {
								cover -= (1 << vit->first);
							}
							cout << "cover: " << j << "\n";
							if(P->feasible[cover]) {
								P->max_children[cover] = max(P->max_children[cover],
																						 it->max_children[j]+bitcount(j));
							}
						}
					}
				}
			}
		}
		}
		int max_S = 0;
		for(int j = 0; j < (1 << s); j++) {
			if(C.back().feasible[j]) {
				max_S = max(C[0].max_children[j] + bitcount(j), max_S);
			}
		}
		cout << max_S << "\n";
	}
}
