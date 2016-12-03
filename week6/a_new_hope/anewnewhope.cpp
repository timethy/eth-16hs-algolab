#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <algorithm>

using namespace std;

class Commando {
public:
	vector<int> children;
	multimap<int, int> supervise;
	map<int, int> supervised;
	vector<size_t> DP;
};

// DP(c, S): One entry means how many troopers can be controlled in that subtree.
// (with the given subset S of troopers in the root)
void DP_entry(vector<Commando>& C, long s, int c, bitset<14> S) {
	Commando& Cc = C[c];
	// check if bitset is feasible:
	bitset<14> S_cover(0);
	for(long t = 0; t < s; t++) {
		if(S.test(t)) {
			multimap<int, int>::iterator it, it_end;
			for(tie(it, it_end) = Cc.supervise.equal_range(t); it != it_end; ++it) {
				S_cover.set(it->second);
			}
		}
	}
	cout << "S:       " << S << endl;
	cout << "S_cover: " << S_cover << endl;
	if((S & S_cover).none()) { // no overlap feasible
		Cc.DP[S.to_ulong()] = S.count();
		// Now compute sum (for every children) of maximal number of troopers per child
		for(vector<int>::iterator it = Cc.children.begin(); it != Cc.children.end(); it++) {
			Commando& Cchild = C[*it];
			size_t m = Cchild.DP[0];
			// For all possible subsets, compute maximum
			for(long i = 0; i < 1<<s; i++) {
				bitset<14> S_child(i);
				bitset<14> KO(0);
				for(map<int, int>::iterator it_ = Cchild.supervised.begin(); it_ != Cchild.supervised.end(); ++it_) {
					if(S.test(it_->second)) {
						KO.set(it_->first);
					}
				}
				if((S_child & KO).none()) {
					m = max(m, Cchild.DP[S_child.to_ulong()]);
				}
			}
			Cc.DP[S.to_ulong()] += m;
		}
	} else {
		Cc.DP[S.to_ulong()] = 0;
	}
}

void DP_compute(vector<Commando>& C, long s, int c) {
	Commando& Cc = C[c];
	// Compute children first
	for(vector<int>::iterator it = Cc.children.begin(); it != Cc.children.end(); it++) {
		DP_compute(C, s, *it);
	}
	// Then compute our entries
	for(long i = 0; i < 1<<s; i++) {
		DP_entry(C, s, c, bitset<14>(i));
	}
}

void testcase() {
	int k, s, m;
	cin >> k >> s >> m;

	vector<Commando> C(k);
	for(int i = 0; i < k; i++) {
		C[i].DP.reserve(1<<s);
		for(int j = 0; j < 1<<s; j++) {
			C[i].DP[j] = 0;
		}
	}

	for(int i = 0; i < m; i++) {
		int u, v, h;
		cin >> u >> v >> h;
		if(u != v) {
			C[u].children.push_back(v);
		}
		for (int j = 0; j < h; j++) {
			int x, y;
			cin >> x >> y;
			if(u != v) {
				C[v].supervised.insert(make_pair(y, x));
			} else {
				C[u].supervise.insert(make_pair(x, y));
			}
		}
	}

	// Compute for all children:
	DP_compute(C, s, 0);

	cout << *max_element(C[0].DP.begin(), C[0].DP.end()) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
