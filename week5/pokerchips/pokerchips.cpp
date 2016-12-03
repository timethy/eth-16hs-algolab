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
			row_length = row_length * (m[i] + 1);
		}
		P.resize(idxOf(m));
	}
	int getPoints(vector<int> &idx) {
		return P[idxOf(idx)];
	};
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
				bool neg_idx = false;
				vector<int> v(n);
				copy(idx.begin(), idx.end(), v.begin());
				for (int i = 0; i < n; i++) {
					if ((s >> i) % 2 == 1) {
						v[i] = v[i] - 1;
						neg_idx |= (v[i] < -1);
					}
				}
				if (neg_idx) {
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
			idx += (i[j] + 1)*rl[j];
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
			v_i[i] = -1;
			v_end[i] = m[i] - 1;
		}
		// Assuming m[d] > 1 for now
		while(v_i != v_end) {
			P.update(v_i);
			// increment
			v_i[0] += 1;
			// carry oveflow
			for(int d = 0; d < n && v_i[d] == m[d]; d++) {
				v_i[d] = -1;
				if (d < n) {
					v_i[d + 1] += 1;
				}
			}
		}
		P.update(v_i);
		cout << P.getPoints(v_end) << "\n";
	}
}
