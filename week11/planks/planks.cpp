// for algolab, by taubnert@ethz.ch
#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

const int B = 20;

vector<vector<int>> partition(int p, int a, bitset<B> taken, vector<int>& l) {
	int n = l.size();
	vector<vector<int>> partitions;
	if(p == 1) {
		int len = 0;
		// take all that are not taken
		auto take = bitset<B>((1<<n)-1) xor taken;
		for (int i = 0; i < n; i++) {
			if (take.test(i)) {
				len += l[i];
			}
		}
		if(len <= a) {
			vector<int> p1;
			p1.push_back(len);
			partitions.push_back(p1);
		}
	} else {
		for (int s = 0; s < (1<<n)-1; s++) {
			auto take = bitset<B>(s);
			if ((take & taken).none()) {
				int len = 0, len_remaining = 0;
				for (int i = 0; i < n; i++) {
					if (take.test(i)) {
						len += l[i];
					} else if(!taken.test(i)) {
						len_remaining += l[i];
					}
				}
				if (len <= a && len_remaining <= (p-1)*a) {
					auto prs = partition(p - 1, a, take | taken, l);
					for (auto &pr : prs) {
						pr.push_back(len);
						partitions.push_back(pr);
					}
				}
			}
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

	vector<vector<int>> V1 = partition(4, a, bitset<B>(0), L1);
	vector<vector<int>> V2 = partition(4, a, bitset<B>(0), L2);

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

	// Split into 2a
	cout << num/24 << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
