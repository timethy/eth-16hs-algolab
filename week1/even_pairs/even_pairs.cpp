#include <iostream>
#include <vector>
#include <map>

using namespace std;

void testcase() {
	unsigned n;
	cin >> n;
	vector<int> x(n);
	vector<int> even(n); // number of even sums that end in i
	vector<int> odd(n); // number of odd sums that end in i
	int n_pairs = 0;

	for (unsigned i = 0; i < n; i++) {
		cin >> x[i];
		if(i == 0) {
			if(x[i] == 0) { even[i] = 1; }
			else { odd[i] = 1; }
		} else {
			if(x[i] == 0) {
				even[i] = even[i-1]+1;
				odd[i] = odd[i-1];
			} else {
				even[i] = odd[i-1];
				odd[i] = even[i-1]+1;
			}
		}
		n_pairs += even[i];
	}

	cout << n_pairs << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}

