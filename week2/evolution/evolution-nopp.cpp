#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Slow intuitive/trivial solution.
// O(n x q)
void testcase() {
	map<string,int> byname;
	int n, q;
	cin >> n >> q;
	vector<string> names(n);
	vector<int> ages(n);
	vector<int> ancestors(n);
	// O(n)
	for (int j = 0; j < n; j++) {
		string name;
		int age;
		cin >> name >> age;
		names[j] = name;
		ages[j] = age;
		ancestors[j] = -1;
		byname[name] = j;
	}
	// O(n x log(n))
	for (int j = 0; j < n-1; j++) {
		string s, p;
		cin >> s >> p;
		ancestors[byname[s]] = byname[p];
	}
	// O(q x n)
	for (int j = 0; j < q; j++) {
		string s;
		int b;
		cin >> s >> b;
		int s0 = byname[s];
		int s1 = ancestors[s0];
		// potentially O(n)
		while(s1 >= 0 && ages[s1] <= b) {
			s0 = s1;
			s1 = ancestors[s1];
		}
		cout << names[s0];
		if (j != q - 1) { cout << " "; }
	}
	cout << "\n";
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}