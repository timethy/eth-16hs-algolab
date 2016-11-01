#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main (void) {
	int t;
	int i, j;
	cin >> t;
	for (i = 0; i < t; i++) {
		map<string,int> byname;
		int n, q;
		cin >> n >> q;
		vector<string> names(n);
		vector<int> ages(n);
		vector<int> ancestors(n);
		for (j = 0; j < n; j++) {
			string name;
			int age;
			cin >> name >> age;
			names[j] = name;
			ages[j] = age;
			ancestors[j] = -1;
			byname[name] = j;
		}
		for (j = 0; j < n-1; j++) {
			string s, p;
			cin >> s >> p;
			ancestors[byname[s]] = byname[p];
		}
		for (j = 0; j < q; j++) {
			string s;
			int b;
			cin >> s >> b;
			int s0 = byname[s];
			int s1 = ancestors[s0];
			while(s1 >= 0 && ages[s1] <= b) {
				s0 = s1;
				s1 = ancestors[s1];
			}
			cout << names[s0];
			if (j != q - 1) { cout << " "; }
		}
		cout << "\n";
	}
}
