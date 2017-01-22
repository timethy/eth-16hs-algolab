// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

inline int num_a(vector<unsigned> c, vector<unsigned> &s, unsigned i, unsigned a) {
	if (c.size() == i) {
		return 1;
	}
	unsigned n = 0;
	for (unsigned c_e = 1; c_e*s[i] <= a && c_e < c[i]; ++c_e) {
		auto c_ = c;
		c_[i] -= c_e;
		n += num_a(c, s, i + 1, a - c_e*s[i]);
	}
	n += num_a(c, s, i + 1, a);
	if (c[i]*s[i] <= a) {
		n += num_a(c, s, i + 1, a - c[i]*s[i]);
	}
	return n;
}

inline int num_2a(vector<unsigned> c, vector<unsigned> &s, unsigned i,
                  vector<unsigned> v, unsigned a, unsigned a_total) {
	if (c.size() == i) {
		if(a == 0) {
			bool lex_bigger = true;
			for (auto v_it = v.begin(), c_it = c.begin();
			     v_it != v.end() && c_it != c.end();) {
				if(*v_it > *c_it) {
					break;
				}
				if(*v_it < *c_it) {
					lex_bigger = false;
					break;
				}
				++v_it;
				++c_it;
			}
			if (lex_bigger) {
				for (auto it = v.begin(); it != v.end(); ++it) {
					cout << *it << " ";
				}
				cout << endl;
				return num_a(c, s, 0, a_total/2)*num_a(v, s, 0, a_total/2);
			}
			for (auto it = c.begin(); it != c.end(); ++it) {
				cout << *it << " ";
			}
			cout << "-";
				for (auto it = v.begin(); it != v.end(); ++it) {
					cout << *it << " ";
				}
			cout << "n" << endl;
			return 0;
		}
		return 0;
	}
	unsigned n = 0;
	for (unsigned c_e = 1; c_e*s[i] <= a && c_e < c[i]; ++c_e) {
		auto c_ = c;
		c_[i] -= c_e;
		v[i] = c_e;
		n += num_2a(c_, s, i + 1, v, a - c_e*s[i], a_total);
	}
	v[i] = 0;
	n += num_2a(c, s, i + 1, v, a, a_total);
	if (c[i]*s[i] <= a) {
		auto c_ = c;
		c_[i] = 0;
		v[i] = c[i];
		n += num_2a(c_, s, i + 1, v, a - c[i]*s[i], a_total);
	}
	return n;
}

void testcase() {
	unsigned n;
	cin >> n;

	unsigned l_total = 0;
	vector<unsigned> ls(n);
	for (unsigned i = 0; i < n; i++) {
		cin >> ls[i];
		l_total += ls[i];
	}
	unsigned a = l_total/4;

	// Idea: count how often which size exists s[i] (size), c[i] (number)
	// Edge must be sum_i s[i]*c_e[i] = a, c_e[i] <= c[i];
	sort(ls.begin(), ls.end());
	reverse(ls.begin(), ls.end());
	vector<unsigned> c(1), s(1);
	int lengths = 1;
	c[0] = 1;
	s[0] = ls[0];
	for (unsigned i = 0; i < n; i++) {
		if (s[lengths - 1] == ls[i]) {
			c[lengths - 1]++;
		} else {
			s.push_back(ls[i]);
			c.push_back(1);
			lengths += 1;
		}
	}
	cout << "a=" << a << endl;

	for (auto it = s.begin(); it != s.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	for (auto it = c.begin(); it != c.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	vector<unsigned> v(lengths);

	// Split into 2a
	cout << num_2a(c, s, 0, v, 2*a, 2*a) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
