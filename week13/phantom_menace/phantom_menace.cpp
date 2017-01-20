// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits <vecS, vecS, directedS> Traits;
typedef adjacency_list <vecS, vecS, directedS, no_property,
property<edge_capacity_t, long,
		property <edge_residual_capacity_t, long,
		property <edge_reverse_t, Traits::edge_descriptor>>>> Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap &capacitymap;
	ReverseEdgeMap &revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap) :
			G(G), capacitymap(capacitymap), revedgemap(revedgemap) {}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, reverseE;
		bool success;
		tie(e, success) = add_edge(from, to, G);
		tie(reverseE, success) = add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[reverseE] = 0;
		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
	}
};

// Below is my code

void testcase() {
	unsigned n, m, s, d;
	cin >> n >> m >> s >> d;

	// Every obj has two nodes
	Graph G(2*n+2);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	EdgeAdder ea(G, capacitymap, revedgemap);

	const auto V_SOURCE = 2*n;
	const auto V_SINK = 2*n+1;

	for (unsigned k = 0; k < n; k++) {
		ea.addEdge(2*k, 2*k+1, 1);
	}
	for (unsigned k = 0; k < m; k++) {
		unsigned i, j;
		cin >> i >> j;
		ea.addEdge(2*i+1, 2*j, 1);
	}
	for (unsigned k = 0; k < s; k++) {
		unsigned i;
		cin >> i;
		ea.addEdge(V_SOURCE, 2*i, 1);
	}
	for (unsigned k = 0; k < d; k++) {
		unsigned j;
		cin >> j;
		ea.addEdge(2*j+1, V_SINK, 1);
	}

	long flow = edmonds_karp_max_flow(G, V_SOURCE, V_SINK);
	cout << flow << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
