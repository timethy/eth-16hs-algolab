#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void testcase() {
	unsigned m;
	cin >> m;

	vector<float> fs(m);
	for(unsigned i = 0; i < m; i++) {
		cin >> fs[i];
	}
	sort(fs.begin(), fs.end());
	float f = 0;
	for(vector<float>::iterator it = fs.begin(); it != fs.end(); it++) {
		f += *it;
	}

	cout << f << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
