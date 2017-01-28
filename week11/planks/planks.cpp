// for algolab, by taubnert@ethz.ch
#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

const int B = 20;

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
	for(int s1 = 0; s1 < (1<<n) - 1; s1++) {
		auto S1 = bitset<B>(s1);
		for(int s2 = 0; s2 < (1<<n) - 1; s2++) {
			auto S2 = bitset<B>(s2);
			for(int s3 = 0; s3 < (1<<n) - 1; s3++) {
				auto S3 = bitset<B>(s3);
				if((S3 & (S1 | S2)).none() && (S1 & S2).none()) {
					auto S4 = ALL xor (S3 | S2 | S1);
					vector<int> lengths(4);
					lengths[0] = (length_of(S4, l));
					lengths[1] = (length_of(S3, l));
					lengths[2] = (length_of(S2, l));
					lengths[3] = (length_of(S1, l));
//					cout << S1 << "|" << S2 << "|" << S3 << "|" << S4 << endl;
					partitions.push_back(lengths);
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
