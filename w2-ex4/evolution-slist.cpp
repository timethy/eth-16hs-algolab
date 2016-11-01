#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	int i, j;
	cin >> t;
	for (i = 0; i < t; i++) {
		map<string,int> byname;
		vector<pair<string,int>> queries_fifo;
		map<string,vector<int>> queries;
		map<string,map<int,int>> query_results;
		int n, q;
		cin >> n >> q;
		vector<string> names(n);
		vector<int> ages(n);
		vector<int> ancestors(n);
		for (j = 0; j < n; j++) { // O(n log n)
			string name;
			int age;
			cin >> name >> age;
			names[j] = name;
			ages[j] = age;
			ancestors[j] = -1;
			byname[name] = j;
		}
		for (j = 0; j < n-1; j++) { // O(n log n)
			string s, p;
			cin >> s >> p;
			ancestors[byname[s]] = byname[p];
		}
		// read in queries
		for (j = 0; j < q; j++) { // O(q log q)
			string s;
			int b;
			cin >> s >> b;
			queries_fifo.push_back(make_pair(s, b));
			queries[s].push_back(b);
		}
		// sort queries ascending by age and then query
		for (map<string,vector<int>>::iterator it(queries.begin());
				 it != queries.end(); it++) {
			sort(it->second.begin(), it->second.end());
			int s0 = byname[it->first];
			int s1 = ancestors[s0];
			map<int, int> results = query_results[it->first];
			for (vector<int>::iterator it_age(it->second.begin());
					 it_age != it->second.end(); it_age++) {
				while (s1 >= 0 && ages[s1] <= *it_age) {
					s0 = s1;
					s1 = ancestors[s1];
				}
				results[*it_age] = s0;
			}
			query_results[it->first] = results;
		}
		for (vector<pair<string,int>>::iterator it(queries_fifo.begin());
		     it != queries_fifo.end(); it++) {
			cout << names[query_results[it->first][it->second]];
			if (it + 1 != queries_fifo.end()) {
				cout << " ";
			}
		}

		cout << "\n";
	}
}

