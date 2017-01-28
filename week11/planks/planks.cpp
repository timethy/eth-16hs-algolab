// for algolab, by taubnert@ethz.ch
#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

const int B = 21;

inline int length_of(bitset<B> bits, vector<int>& l) {
	int len = 0;
	for(unsigned i = 0; i < l.size(); i++) {
		if(bits.test(i)) {
			len += l[i];
		}
	}
	return len;
}

vector<vector<int>> partition(int a, vector<int>& l) {
	int n = l.size();
	vector<vector<int>> partitions;
	auto ALL = bitset<B>((1<<n) - 1);
	for(int s1 = 0; s1 < (1<<n); s1++) {
		auto S1 = bitset<B>(s1);
		for(int s2 = 0; s2 < (1<<n); s2++) {
			auto S2 = bitset<B>(s2);
			auto F1 = S1 & S2; // In both sets
			auto F2 = S2 & (ALL xor S1); // In S1 but not S2
			auto F3 = S1 & (ALL xor S2); // In S2 but not S1
			auto F4 = ALL xor (S1 | S2); // In neither sets
			vector<int> ls(4);
			ls[0] = length_of(F1, l);
			ls[1] = length_of(F2, l);
			ls[2] = length_of(F3, l);
			ls[3] = length_of(F4, l);
//			if(ls[0] >= ls[1] && ls[1] >= ls[2] && ls[2] >= ls[3]) {
				if (*max_element(ls.begin(), ls.end()) <= a) {
					partitions.push_back(ls);
				}
//			}
		}
	}
	return partitions;
}

void testcase() {
	int n;
	cin >> n;

	int l_total = 0;
	vector<int> ls(n);
	for (int i = 0; i < n; i++) {
		cin >> ls[i];
		l_total += ls[i];
	}
	int a = l_total/4;

	// First sort by size, bigger first
	sort(ls.rbegin(), ls.rend());

	int N1 = n/2;
	int N2 = n - N1;

	vector<int> L1(N1);
	vector<int> L2(N2);

	copy(ls.begin(), ls.begin()+N1, L1.begin());
	copy(ls.begin()+N1, ls.end(), L2.begin());

	// lengths are always largest first
	vector<vector<int>> V1 = partition(a, L1);
	vector<vector<int>> V2 = partition(a, L2);

	sort(V2.begin(), V2.end());

	long num = 0;
	for(auto it = V1.begin(); it != V1.end(); it++) {
		vector<int> p = *it;
		vector<int> p_missing;
		for(auto itp = p.rbegin(); itp != p.rend(); itp++) {
			p_missing.push_back(a-*itp);
		}
		vector<vector<int>>::iterator lb, ub;
		tie(lb, ub) = equal_range(V2.begin(), V2.end(), p_missing);
		num += (ub - lb);
	}

	cout << num/24 << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
