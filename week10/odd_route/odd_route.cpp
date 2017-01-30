#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Namespaces
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int>> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase() {
	unsigned n, m;
	cin >> n >> m;

	unsigned src, tar;
	cin >> src >> tar;

	Graph G(n);
	WeightMap weights = get(edge_weight, G);
	for(unsigned i = 0; i < m; i++) {
		unsigned s, t, w;
		cin >> s >> t >> w;
		Edge e;
		bool success;
		tie(e, success) = add_edge(s, t, G);
		weights[e] = w;
	}

	vector<int> even_even(n, INT_MAX);
	vector<int> even_odd(n, INT_MAX);
	vector<int> odd_even(n, INT_MAX);
	vector<int> odd_odd(n, INT_MAX);

	// Dijkstra:
	priority_queue<pair<int, int>> Q;
	Q.push(make_pair(0, src));
	even_even[src] = 0;
	while(!Q.empty()) {
		int dist, u;
		tie(dist, u) = Q.top();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = target(*ebeg, G);
			// Only follow edges with spare capacity
			bool updated = false;
			int min_dist = INT_MAX;
			if (even_even[u] < INT_MAX) {
				if (weights[*ebeg] % 2 == 1) { // new weight is odd
					int new_dist = even_even[u] + weights[*ebeg];
					if(new_dist < odd_odd[v]) {
						odd_odd[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				} else { // new weight is even
					int new_dist = even_even[u] + weights[*ebeg];
					if(new_dist < odd_even[v]) {
						odd_even[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				}
			}
			if (even_odd[u] < INT_MAX) {
				int new_dist = even_odd[u] + weights[*ebeg];
				if (weights[*ebeg] % 2 == 0) {
					if(new_dist < odd_odd[v]) {
						odd_odd[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				} else { // new weight is odd
					if(new_dist < odd_even[v]) {
						odd_even[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				}
			}
			if (odd_even[u] < INT_MAX) {
				int new_dist = odd_even[u] + weights[*ebeg];
				if (weights[*ebeg] % 2 == 1) { // new weight is odd
					if(new_dist < even_odd[v]) {
						even_odd[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				} else {
					if(new_dist < even_even[v]) {
						even_even[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				}
			}
			if (odd_odd[u] < INT_MAX) {
				int new_dist = odd_odd[u] + weights[*ebeg];
				if (weights[*ebeg] % 2 == 0) {
					if(new_dist < even_odd[v]) {
						even_odd[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				} else { // new weight is odd
					if(new_dist < even_even[v]) {
						even_even[v] = new_dist;
						updated = true;
						min_dist = min(min_dist, new_dist);
					}
				}
			}
			if(updated) {
				Q.push(make_pair(min_dist, v));
			}
		}
	}

	if(odd_odd[tar] != INT_MAX) {
		cout << odd_odd[tar] << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) testcase();
	return 0;
}
