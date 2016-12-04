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
	vector<long> DP; // INIT to -1
};

int sum_children(vector<Commando>& C, int s, vector<int>& children, bitset<14> S) {
	int sum = 0;
	for(vector<int>::iterator it = children.begin(); it != children.end(); it++) {
		Commando& Cchild = C[*it];
		// Compute KO set
		bitset<14> KO(0);
		for(map<int, int>::iterator it_ = Cchild.supervised.begin(); it_ != Cchild.supervised.end(); ++it_) {
			if(S.test(it_->second)) {
				KO.set(it_->first);
			}
		}
		// flip'em
		bitset<14> feasible = KO xor bitset<14>((1<<s) - 1);
		sum += Cchild.DP[feasible.to_ulong()];
	}
	return sum;
}

// DP(c, S): One entry means how many troopers can be controlled in that subtree.
// (with the given subset S of troopers in the root)
long DP_entry(vector<Commando>& C, unsigned s, int c, bitset<14> S) {
	Commando& Cc = C[c];
	if(Cc.DP[S.to_ulong()] >= 0) {
		// Already computed
		return Cc.DP[S.to_ulong()];
	}
	// check if bitset is feasible:
	bitset<14> S_cover(0);
	for(size_t t = 0; t < s; t++) {
		if(S.test(t)) {
			multimap<int, int>::iterator it, it_end;
			for(tie(it, it_end) = Cc.supervise.equal_range(t); it != it_end; ++it) {
				S_cover.set(it->second);
			}
		}
	}
	long x = 0;
	if((S & S_cover).none()) { // no overlap => feasible
		x = S.count() + sum_children(C, s, Cc.children, S);
	} else {
		x = 0;
	}
	// Now compute entry for all subsets and take maximum of it
	for(size_t i = 0; i < s; i++) {
		if(S.test(i)) {
			bitset<14> S_ = S;
			S_.reset(i);
			x = max(x, DP_entry(C, s, c, S_));
		}
	}
	Cc.DP[S.to_ulong()] = x;
	return x;
}

void DP_compute(vector<Commando>& C, unsigned s, int c) {
	Commando& Cc = C[c];
	// Compute children first
	for(vector<int>::iterator it = Cc.children.begin(); it != Cc.children.end(); it++) {
		DP_compute(C, s, *it);
	}
	// Then compute our entries
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
