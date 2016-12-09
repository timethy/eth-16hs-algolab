// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

// MESSY, incompleted ideas:

/*
int combinations(int a, vector<unsigned> &ls, bitset<20> used, int edge) {
	if(edge == 4) {
		return 1;
	} else {
		// all subsets:
		int l_count = ls.size();
		int feasible = 0;
		for (int i = 0; i < (1 << l_count); i++) {
			bitset<20> new_edges(i);
			if ((used & new_edges).none()) {
				// return 0 if infeasible, 1 if feasible
				int b = 0;
				for (unsigned i = 0; i < ls.size(); i++) {
					if (new_edges[i]) {
						b += ls[i];
					}
				}
				if (a == b) {
//					cout << edge << " " << used << " " << new_edges << endl;
					feasible += combinations(a, ls, used | new_edges, edge + 1);
				}
			}
		}
		return feasible;
	}
}

vector<bitset<20>> all_feasible_subsets(int a, vector<unsigned> &ls) {
	vector<bitset<20>> feasible;
	for (int i = 0; i < (1 << ls.size()); i++) {
		bitset<20> edges(i);
		int b = 0;
		for (unsigned i = 0; i < ls.size(); i++) {
			if (edges[i]) {
				b += ls[i];
			}
		}
		if (a == b) {
			feasible.push_back(edges);
		}
	}
	return feasible;
}
*/
/*
bool feasible_vec(int a, vector<unsigned> &ls, vector<bitset<20>> &edges) {
	bool feasible;
	for (vector<bitset<20>>::iterator it = edges.begin(); it != edges.end(); ++it) {
		bitset<20> edge = *it;
		int b = 0;
		for (unsigned i = 0; i < ls.size(); i++) {
			if (edge[i]) {
				b += ls[i];
			}
		}
		feasible &= (a == b);
	}
	return feasible;
}
*/
bool feasible(int a, vector<unsigned> &ls, bitset<20> edge) {
	int b = 0;
	for (unsigned i = 0; i < ls.size(); i++) {
		if (edge[i]) {
			b += ls[i];
		}
	}
	return a == b;
}
bool feasible_2(int a, vector<unsigned> &ls, bitset<20> edge) {
	int b = 0, b_ = 0;
	for (unsigned i = 0; i < ls.size(); i++) {
		if (edge[i]) {
			b += ls[i];
		} else {
			b_ += ls[i];
		}
	}
	return a == b && a == b_;
}
int count(int a, vector<unsigned> &ls, vector<int>& assign, int idx) {
	if(idx > ls.size()) {
		return 0;
	}
	if(idx == ls.size()) {
		bool f = true;
		bitset<20> chosen;
		for (int j = 0; j < 3; j++) {
			bitset<20> edge;
			for (int i = 0; i < ls.size(); i++) {
				if(assign[i] == j) {
					edge.set(i);
				}
			}
			f &= feasible(a, ls, edge);
			chosen &= edge;
		}
		cout << chosen << endl;
		f &= feasible(a, ls, chosen.flip());
		return f ? 1 : 0;
	}
	int c = 0;
	vector<int> assign_ = assign;
	for (int j = 0; j < 3; j++) {
		assign_[idx] = j;
		c += count(a, ls, assign, idx+2);
		c += count(a, ls, assign_, idx+1);
	}
	return c;
}

int count(int a, vector<unsigned> &ls) {

}

/*int count(int a, vector<unsigned> &ls) {
	int c = 0;
	vector<bitset<20>> edge;
	bitset<20> FLIP((1<<ls.size()) - 1);
	for (int i = 0; i < (1 << ls.size()); i++) {
		bitset<20> two_edges(i);
		int t = two_edges.count();
		if(t <= ls.size/2 && feasible(2*a, ls, two_edges)) {
			// Now take subedge:
			for (int j = 0; j < (1 << t); j++) {
				bitset<20> filter(j);
				bitset<20> edge1, edge2, edge3, edge4;
				int x = 0;
				for (int k = 0; k < ls.size(); k++) {
					if (two_edges.test(k)) {

						x++;
					} else {

					}
				}
				bitset<20> edge1 = two_edges & filter,
						edge2 = (FLIP xor two_edges) & filter,
						edge3 = two_edges xor edge1,
						edge4 = (FLIP xor two_edges) xor edge2;
/**				cout << edge1 << " " << edge3 << " " << edge2 << " " << edge4 << endl;
				cout << feasible(a, ls, edge1) << " "
				     << feasible(a, ls, edge2) << " "
				     << feasible(a, ls, edge3) << " "
				     << feasible(a, ls, edge4) << " " << endl;
				if (feasible(a, ls, edge1) &&
				    feasible(a, ls, edge2) &&
				    feasible(a, ls, edge3) &&
				    feasible(a, ls, edge4)) {
					c += 1;
				}
			}
		}
	}
	return c;
}*/

void testcase() {
	unsigned n;
	cin >> n;

	unsigned l_total = 0;
	vector<unsigned> ls(n);
	for (unsigned i = 0; i < n; i++) {
		cin >> ls[i];
		l_total += ls[i];
	}

	// square_size must be l_total/4
	int a = l_total / 4;
//	int c = combinations(a, ls, bitset<20>(0), 0);
/*	vector<bitset<20>> sets = all_feasible_subsets(a, ls);
	map<unsigned long, bool> feasible;
	for (vector<bitset<20>>::iterator it = sets.begin(); it != sets.end(); it++) {
		feasible.insert(make_pair(it->to_ulong(), true));
		cout << *it << endl;
	}*/

	// Now take 4 sets and check if there are disjoint:
	vector<int> empty(n);
	int c = count(a, ls, empty, 0);
	cout << c << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
