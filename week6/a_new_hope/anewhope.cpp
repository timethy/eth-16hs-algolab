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
	multimap<int, int> supervised;
	vector<long> DP; // INIT to -1
};

int sum_children(vector<Commando>& C, int s, vector<int>& children, bitset<14> S) {
	int sum = 0;
	for(vector<int>::iterator it = children.begin(); it != children.end(); it++) {
		Commando& Cchild = C[*it];
		// Compute covered set
		bitset<14> covered(0);
		for(multimap<int, int>::iterator it_ = Cchild.supervised.begin(); it_ != Cchild.supervised.end(); ++it_) {
			if(S.test(it_->second)) {
				covered.set(it_->first);
			}
		}
		// flip'em
		bitset<14> uncovered = covered xor bitset<14>((1<<s) - 1);
		sum += Cchild.DP[uncovered.to_ulong()];
	}
	return sum;
}

// DP(c, S): One entry means how many troopers can be controlled in that subtree.
// (with the given subset S of troopers in the root)
// Computing a row of DP for one Commando center c should take
// O(2^s * s * c), where c is the number of children of said Commando center
// Since every commando has only one parent, the total runtime is in O(2^s * s * k)
long DP_entry(vector<Commando>& C, unsigned s, int c, bitset<14> S) {
	Commando& Cc = C[c];
	if(Cc.DP[S.to_ulong()] >= 0) {
		// Already computed
		return Cc.DP[S.to_ulong()];
	} else {
		// check if bitset is feasible:
		bitset<14> S_cover(0);
		for (size_t t = 0; t < s; t++) {
			if (S.test(t)) {
				auto range = Cc.supervise.equal_range(t);
				for (auto it = range.first; it != range.second; ++it) {
					S_cover.set(it->second);
				}
			}
		}
		long x = 0;
		if ((S & S_cover).none()) { // no overlap => feasible
			x = S.count() + sum_children(C, s, Cc.children, S);
		}
		// Now compute entry for all (direct) subsets and take maximum of it
		for (size_t i = 0; i < s; i++) {
			if (S.test(i)) {
				bitset<14> S_ = S;
				S_.reset(i);
				x = max(x, DP_entry(C, s, c, S_));
			}
		}
		Cc.DP[S.to_ulong()] = x;
		return x;
	}
}

void DP_compute(vector<Commando>& C, unsigned s, int c) {
	Commando& Cc = C[c];
	// Compute children first
	for(vector<int>::iterator it = Cc.children.begin(); it != Cc.children.end(); it++) {
		DP_compute(C, s, *it);
	}
	// Then compute our entries, start computation at largest subset
	DP_entry(C, s, c, bitset<14>((1<<s) - 1));
}

void testcase() {
	unsigned k, s, m;
	cin >> k >> s >> m;

	vector<Commando> C(k);
	for(unsigned i = 0; i < k; i++) {
		C[i].DP.reserve(1<<s);
		for(int j = 0; j < 1<<s; j++) {
			C[i].DP[j] = -1;
		}
	}

	for(unsigned i = 0; i < m; i++) {
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

	cout << C[0].DP[(1<<s) - 1] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
