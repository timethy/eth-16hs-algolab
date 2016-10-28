#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <sstream>

using namespace std;

class Stacks {
public:
	Stacks(int n, vector<int> m, vector<vector<int>> &C) {
		this->n = n;
		this->m = m;
		this->C = C;
		int row_length = 1;
		this->rl.resize(n);
		for(int i = 0; i < n; i++) {
			this->rl[i] = row_length;
			row_length = row_length * m[i];
		}
		P.resize(idxOf(m));
	}
	int getPoints(vector<int> &idx) {
		return P[idxOf(idx)];
	};
	// TODO: when one column is empty can still carry on
	void update(vector<int> &idx) {
		int max_points = 0;
		// for all s in 2^n, check idx i-s
		for (int s = 1; s < (1 << n); s++) {
			int k = 0;
			bool same_color = true;
			int color = 0;
			for (int i = 0; i < n; i++) {
				if ((s >> i) % 2 == 1) {
					if (color == 0) { color = C[i][idx[i]]; }
					same_color &= (color == C[i][idx[i]]);
					k += 1;
				}
			}
			if(same_color) {
				vector<int> v(n);
				copy(idx.begin(), idx.end(), v.begin());
				for (int i = 0; i < n; i++) {
					if ((s >> i) % 2 == 1) {
						v[i] = v[i] - 1;
						valid &= (idx[i] > 0);
					}
				}
				if (!valid) {
					if (k > 1) {
						max_points = max(1 << (k - 2), max_points);
					}
				} else if (k > 1) {
					max_points = max(P[idxOf(v)] + (1 << (k - 2)), max_points);
				} else {
					max_points = max(P[idxOf(v)], max_points);
				}
			}
		}
		P[idxOf(idx)] = max_points;
	};
private:
	int n;
	vector<int> m;
	vector<int> rl;
	vector<int> P;
	vector<vector<int>> C;
	int idxOf(vector<int> &i) {
		int idx = 0;
		for(int j = 0; j < n; j++) {
			idx += i[j]*rl[j];
		}
		return idx;
	}
};

int main (void) {
	int t;
	cin >> t;

	while (t--) {
		int n;
		cin >> n;

		vector<int> m(n);
		vector<vector<int>> C(n);

		// read in m
		for (int i = 0; i < n; i++) {
			cin >> m[i];
		}
		// read in chips
		for (int i = 0; i < n; i++) {
			C[i].resize(m[i]);
			for (int j = 0; j < m[i]; j++) {
				cin >> C[i][j];
			}
		}

		// This is P stored flatly
		Stacks P(n, m, C);

		// NOW, DP FUN
		vector<int> v_i(n);
		vector<int> v_end(n);
		for (int i = 0; i < n; i++)  {
			v_i[i] = 0;
			v_end[i] = m[i] - 1;
		}
		// Assuming m[d] > 1 for now
		while(v_i != v_end) {
			P.update(v_i);
			int di = 0;
			while(m[di] == 1) {
				di++;
			}
			// increment
			v_i[di] += 1;
			// carry oveflow
			for(int d = di; d < n && v_i[d] == m[d];) {
				v_i[d] = 0;
				do {
					d++;
				} while(d < n && m[d] == 1);
				v_i[d] += 1;
			}
		}
		P.update(v_i);
		//cout << "\nv_i:" << P.getPoints(v_i) << "\n";
		for(vector<int>::iterator it = v_end.begin();
				it != v_end.end(); it++) {
		//	cout << *it << " ";
		}
		//cout << "\nv_end:";
		cout << P.getPoints(v_end) << "\n";
	}
}
