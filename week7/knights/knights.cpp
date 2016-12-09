#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Namespaces
using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor>>>> Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor Edge;

void addEdge(int from, int to, long c, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap, Graph &G) {
	Edge e, reverseE;
	bool success;
	tie(e, success) = add_edge(from, to, G);
	tie(reverseE, success) = add_edge(to, from, G);
	capacitymap[e] = c;
	capacitymap[reverseE] = 0;
	revedgemap[e] = reverseE;
	revedgemap[reverseE] = e;
}

int idx(int cols, int i, int j) {
	return cols * i + j;
}

void testcase() {
	int m, n, k, c;
	cin >> m >> n >> k >> c;
	Graph G(2 * n * m + 2);
	EdgeCapacityMap cap = get(edge_capacity, G);
	ReverseEdgeMap rev = get(edge_reverse, G);

	if (m == 0 || n == 0) {
		cout << 0 << endl;
		return;
	}

	int v_source = 2 * n * m;
	int v_sink = 2 * n * m + 1;
	for (int i = 0; i < k; i++) {
		int x, y;
		cin >> x;
		cin >> y;
		addEdge(v_source, 2 * idx(m, y, x), 1, cap, rev, G);
	}
	// corner
	addEdge(2 * idx(m, 0, 0) + 1, v_sink, 2, cap, rev, G);
	addEdge(2 * idx(m, 0, m - 1) + 1, v_sink, 2, cap, rev, G);
	addEdge(2 * idx(m, n - 1, 0) + 1, v_sink, 2, cap, rev, G);
	addEdge(2 * idx(m, n - 1, m - 1) + 1, v_sink, 2, cap, rev, G);
	// left and right to sink
	for (int i = 1; i < n - 1; i++) {
		addEdge(2 * idx(m, i, 0) + 1, v_sink, 1, cap, rev, G);
		addEdge(2 * idx(m, i, m - 1) + 1, v_sink, 1, cap, rev, G);
	}
	// bottom and top to sink
	for (int j = 1; j < m - 1; j++) {
		addEdge(2 * idx(m, 0, j) + 1, v_sink, 1, cap, rev, G);
		addEdge(2 * idx(m, n - 1, j) + 1, v_sink, 1, cap, rev, G);
	}
	// The grid
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			addEdge(2 * idx(m, i, j), 2 * idx(m, i, j) + 1, c, cap, rev, G);
			for (int i_ = i - 1; i_ <= i + 1; i_++) {
				for (int j_ = j - 1; j_ <= j + 1; j_++) {
					if (abs(i - i_) + abs(j - j_) == 1 &&
					    i_ >= 0 && j_ >= 0 &&
					    i_ < n && j_ < m) {
						addEdge(2 * idx(m, i, j) + 1, 2 * idx(m, i_, j_), 1, cap, rev, G);
					}
				}
			}
		}
	}

	// Compute max flow
	int max_flow = push_relabel_max_flow(G, v_source, v_sink);
	cout << max_flow << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;    // First input line: Number of testcases.
	while (t--) { testcase(); }
	return 0;
}

