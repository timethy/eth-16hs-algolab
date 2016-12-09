#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list <vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

double dist_squared(int x1, int y1, int x2, int y2) {
	double xd = (x2 - x1);
	double yd = (y2 - y1);
	return sqrt(xd * xd + yd * yd);
}

int max_connected_component(vector <vector<int>> &adj, int l, int n) {
	int V = n - l;
	Graph G(V);
	// Build graph
	for (int i = l; i < n; i++) {
		for (vector<int>::iterator it = adj[i].begin(); it != adj[i].end(); ++it) {
			Edge e;
			bool success;
			tie(e, success) = add_edge(i - l, *it - l, G);
			tie(e, success) = add_edge(*it - l, i - l, G);
		}
	}
	// Find connected components
	vector<int> sccmap(V);
	int nscc = connected_components(G, make_iterator_property_map(sccmap.begin(), get(vertex_index, G)));
	// Get biggest connected component
	int m = 0;
	for (int i = 0; i < nscc; i++) {
		int m_ = count(sccmap.begin(), sccmap.end(), i);
		m = max(m, m_);
	}
	return m;
}

void testcase(int t) {
	int n, r;
	cin >> n >> r;

	vector<int> xs(n);
	vector<int> ys(n);

	vector <vector<int>> adj(n);

	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		xs[i] = x;
		ys[i] = y;
	}

	// Reachabilities
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (dist_squared(xs[i], ys[i], xs[j], ys[j]) <= r) {
				// For every node store edges referring to later edges:
				adj[i].push_back(j);
			}
		}
	}

	// Binary search
	int kl = 0; // the first feasible
	int kr = n / 2 + 1; // the first unfeasible
	while (kr - kl > 1) {
		int k = kl + (kr - kl) / 2;
		int m = max_connected_component(adj, k, n);
		if (k <= m) {
			kl = k;
		} else {
			kr = k;
		}
	}

	cout << kl << endl;
}

int main(void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		testcase(i);
	}
}

