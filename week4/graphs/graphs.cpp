#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Namespaces
using namespace std;
using namespace boost;

// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS, no_property,
		property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator EdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase() {
	int n, m;
	cin >> n >> m;
	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
	for (int i = 0; i < m; ++i) {
		int u, v, w;
		cin >> u >> v >> w;
		Edge e;
		bool success;			
		tie(e, success) = add_edge(u, v, G);
		weightmap[e] = w;
	}

	vector<Edge> mst_edges;
	kruskal_minimum_spanning_tree(G, back_inserter(mst_edges));

	int w = 0;
	for(vector<Edge>::iterator it = mst_edges.begin(); it != mst_edges.end(); it++) {
		w += weightmap[*it];
	}
	cout << w << endl;

	vector<Vertex> predecessors(num_vertices(G));
	vector<int> distances(num_vertices(G));
	dijkstra_shortest_paths(G, 0,
		predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, G))).
		distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, G))));

	int d = 0;
	for(vector<int>::iterator it = distances.begin(); it != distances.end(); it++) {
		if(*it != INT_MAX) {
			d = max(d, *it);
		}
	}
	cout << d << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;	// First input line: Number of testcases.
	while(t--) { testcase(); }
	return 0;
}

