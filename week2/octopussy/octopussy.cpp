// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void testcase() {
	unsigned n;
	cin >> n;

	vector<unsigned> t(n);
	for(unsigned i = 0; i < n; i++) {
		cin >> t[i];
	}
	// Preprocess deadlines
	for (unsigned i = 0; i <= (n-3)/2; i++) {
		t[2*i+1] = min(t[2*i+1], t[i]-1);
		t[2*i+2] = min(t[2*i+2], t[i]-1);
	}
	sort(t.begin(), t.end());
	// compare to [1,...,n]
	bool ok = true;
	for(unsigned i = 0; i < n; i++) {
		ok &= t[i] >= i + 1;
	}

	if(ok) {
		cout << "yes" << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
