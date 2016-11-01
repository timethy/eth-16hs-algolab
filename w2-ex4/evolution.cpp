#include <iostream>
#include <map>

using namespace std;

struct species {
	int age;
	string name;
	species* ancestor;
};

int main (void) {
	int t;
	int i, j;
	cin >> t;
	for (i = 0; i < t; i++) {
		map<string,species*> byname;
		int n, q;
		cin >> n >> q;
		for (j = 0; j < n; j++) {
			string name;
			int age;
			species* s = (species*) malloc(sizeof(species));
			cin >> name >> age;
			s->name = name;
			s->age = age;
			s->ancestor = NULL;
			byname[name] = s;
		}
		for (j = 0; j < n-1; j++) {
			string s, p;
			cin >> s >> p;
			if(byname[s] && byname[p]) {
			byname[s]->ancestor = byname[p];
			}
		}
		for (j = 0; j < q; j++) {
			string s;
			int b;
			cin >> s >> b;
			species* s0 = byname[s];
			if(s0 && s0->ancestor) {
				species* s1 = s0;
				while(s1 != NULL && s1->age <= b) {
					s0 = s1;
					s1 = s1->ancestor;
				}
			}
			cout << s0->name << " ";
		}
		byname.clear();
	}
}
