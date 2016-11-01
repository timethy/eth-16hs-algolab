#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main (void) {
	int t;
	int i;
	cin >> t;
	for (i = 0; i < t; i++) {
		int n;
		cin >> n;
		vector<int> x(n);

		for (int j = 0; j < n; j++) {
			int x_i;
			cin >> x_i;
			x.push_back(x_i);
		}

		int n_pairs = 0;
		int subset_sum = 0;

		for (vector<int>::iterator it = x.begin(); it != x.end(); it++) {
			if(*it == 0) {
				even_length += 1;
			} else {
				even_length = 0;
			}
			n_pairs += even_length;
		}

		cout << n_pairs;

		cout << "\n";
	}
}
