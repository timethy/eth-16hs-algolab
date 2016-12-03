#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <algorithm>

using namespace std;

class Commando {
public:
	vector<size_t> DP;
	multimap<int, int> supervise;
	int Cp;
	vector<int> supervised;
	vector<int> children;
};

size_t DFS_DP(vector<Commando>& C, long s, int c, bitset<14> S) {
	Commando& Cc = C[c];
	// Compute for all children:
	for(long i = 0; i < (1 << s); i++) {
		Cc.DP[i] = 0;
		for(vector<int>::iterator it = Cc.children.begin(); it != Cc.children.end(); it++) {
			bitset<14> S_(i);
			Cc.DP[i] = max(DFS_DP(C, s, *it, S_), Cc.DP[S_.to_ulong()]);
		}
	}
	// for every subset S in parent, compute maximum amount of troopers
	// Set all supervised as KO
	bitset<14> KO(0);
	for(long j = 0; j < s; j++) {
		if(Cc.supervised[j] >= 0 && S.test(Cc.supervised[j])) {
			KO.set(j);
		}
	}
	size_t m = Cc.DP[0];
	cout << "c=" << c << endl;
	cout << "S: " << S << ", initial max. " << m << endl;
	bitset<14> feasible = KO xor bitset<14>((1 << s) - 1);
	cout << "feasible: " << feasible << endl;
	for(long j = 0; j < 1<<s; j++) {
		bitset<14> bs(j);
		bitset<14> supervised(0);
		for(long t = 0; t < s; t++) {
			if(bs.test(t)) {
				multimap<int, int>::iterator it, it_end;
				tie(it, it_end) = Cc.supervise.equal_range(t);
				for(; it != it_end; ++it) {
					supervised.set(it->second);
				}
			}
		}
		if(((bs | feasible) == feasible) && ((bs & supervised) == bitset<14>(0))) {
			auto v = bs.count() + Cc.DP[bs.to_ulong()];
			m = max(m, v);
			cout << "bs: " << bs << ", v: " << v << ", " << Cc.DP[bs.to_ulong()] << endl;
		}
	}
	cout << "S: " << S << ", max. " << m << endl;
	return m;
}

void testcase() {
	int k, s, m;
	cin >> k >> s >> m;

	vector<Commando> C(k+1);
	for(int i = 0; i < C.size(); i++) {
		C[i].supervised.reserve(s);
		C[i].DP.reserve(1 << s);
		for(int j = 0; j < C.size(); j++) {
			C[i].supervised[j] = -1;
		}
		for(int j = 0; j < 1<<s; j++) {
			C[i].DP[j] = 0;
		}
	}

	for(int i = 0; i < m; i++) {
		int u, v, h;
		cin >> u >> v >> h;
		if(u != v) {
			C[v].Cp = u;
			C[u].children.push_back(v);
		}
		for (int j = 0; j < h; j++) {
			int x, y;
			cin >> x >> y;
			if(u != v) {
				C[v].supervised[y] = x;
			} else {
				C[u].supervise.insert(make_pair(x, y));
			}
		}
	}
	size_t mv = 0;
//	for(int i = 0; i < 1<<s; i++) {
//		bitset<14> S(i);
		mv = max(DFS_DP(C, s, 0, bitset<14>(0)), mv);
//	}

	cout << mv << endl;
	cout << "#############" << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
