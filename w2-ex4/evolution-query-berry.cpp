#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>

using namespace std;

class Species {
public:
	bool operator< (const Species& other) {
		return age <= other.age;
	}
	int age;
	string name;
	int ancestor;
};

bool comp_first(pair<int,string> p1, pair<int,string> p2) {
	return p1.first <= p2.first;
}

deque<pair<int,string>> merge (deque<pair<int,string>> &v1, deque<pair<int, string>> &v2) {
	deque<pair<int,string>> v;
	while(!v1.empty() && !v2.empty()) {
		if (v1.front() <= v2.front()) {
			v.push_back(v1.front());
			v1.pop_front();
		} else {
			v.push_back(v2.front());
			v2.pop_front();
		}
	}
	while(!v1.empty()) {
		v.push_back(v1.front());
		v1.pop_front();
	}
	while(!v2.empty()) {
		v.push_back(v2.front());
		v2.pop_front();
	}
	return v;
}

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	int i, j;
	cin >> t;
	for (i = 0; i < t; i++) {
		int n, q;
		cin >> n >> q;

		vector<Species> species(n);
		map<string,int> byname;

		vector<pair<string,int>> queries_fifo;
		map<string,deque<pair<int,string>>> queries;
		map<string,map<int,int>> query_results;

		for (j = 0; j < n; j++) { // O(n log n)
			string name;
			int age;
			cin >> name >> age;
			species[j].name = name;
			species[j].age = age;
			species[j].ancestor = -1;
			byname[name] = j;
		}
		for (j = 0; j < n-1; j++) { // O(n log n)
			string s, p;
			cin >> s >> p;
			species[byname[s]].ancestor = byname[p];
		}
		// read in queries
		for (j = 0; j < q; j++) { // O(q log q)
			string s;
			int b;
			cin >> s >> b;
			queries_fifo.push_back(make_pair(s, b));
			queries[s].push_back(make_pair(b, s));
		}
		// sort by age for every name
		for (map<string,deque<pair<int,string>>>::iterator it(queries.begin());
				 it != queries.end(); it++) {
			vector<pair<int,string>> v (it->second.begin(), it->second.end());
			sort(v.begin(), v.end(), comp_first);
			copy(v.begin(), v.end(), it->second.begin());
		}

		vector<Species> species_(n);
		// O(n)
		copy(species.begin(), species.end(), species_.begin());
		// O(n log n)
		sort(species_.begin(), species_.end());
		
		for(vector<Species>::iterator it(species_.begin());
		    it != species_.end(); it++) {
			deque<pair<int,string>> cq;
			if(queries.find(it->name) != queries.end() && !(cq = queries[it->name]).empty()) {
				int s0 = byname[it->name];
				int s1 = s0;
				// traverse to top
				while(s1 >= 0) {
					int a = species[s1].age;
					// Check head of current queries if max_age is smaller than s1's age
					while(!cq.empty()) {
						pair<int,string> p = cq.front();
						if(p.first < a) {
							query_results[p.second][p.first] = s0;
							cq.pop_front();
						} else {
							break;
						}
					}
					// Now merge in all queries starting at s1
					if(queries.find(species[s1].name) != queries.end()) {
						deque<pair<int,string>> nq = queries[species[s1].name];
						cq = merge(cq,nq);
						queries.erase(species[s1].name);
					}
					// go one step up
					s0 = s1;
					s1 = species[s1].ancestor;
				}
				// output the rest
				while(!cq.empty()) {
					pair<int,string> p = cq.front();
					query_results[p.second][p.first] = s0;
					cq.pop_front();
				}
				queries.erase(it->name);
			}
		}
		for (vector<pair<string,int>>::iterator it(queries_fifo.begin());
		     it != queries_fifo.end(); it++) {
			cout << species[query_results[it->first][it->second]].name;
			if (it + 1 != queries_fifo.end()) {
				cout << " ";
			}
		}

		cout << "\n";
	}
}

