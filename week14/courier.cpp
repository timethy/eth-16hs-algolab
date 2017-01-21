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
		property < edge_residual_capacity_t, long,
		property < edge_reverse_t, Traits::edge_descriptor>>>>
Graph;
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
	Edge addEdge(int from, int to, long capacity) {
		Edge e, reverseE;
		bool success;
		tie(e, success) = add_edge(from, to, G);
		tie(reverseE, success) = add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[reverseE] = 0;
		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
		return e;
	}
};

// Below is my code

void testcase() {
	unsigned Z, J;
	cin >> Z >> J;

	vector<unsigned> c(Z);
	vector<unsigned> p(J);
	vector <Edge> job_edges(J);

	Graph G(Z + J + 2);
	EdgeCapacityMap capacities = get(edge_capacity, G);
	ResidualCapacityMap res_capacities = get(edge_residual_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	EdgeAdder ea(G, capacities, revedgemap);

	const auto V_SOURCE = Z + J;
	const auto V_SINK = Z + J + 1;

	for (unsigned i = 0; i < Z; i++) {
		cin >> c[i];
		ea.addEdge(V_SOURCE, i, c[i]);
	}
	for (unsigned i = 0; i < J; i++) {
		cin >> p[i];
		job_edges[i] = ea.addEdge(Z + i, V_SINK, p[i]);
	}
	for (unsigned i = 0; i < J; i++) {
		unsigned N;
		cin >> N;
		for (unsigned j = 0; j < N; j++) {
			unsigned zone;
			cin >> zone;
			ea.addEdge(zone, Z + i, c[zone]);
		}
	}

	edmonds_karp_max_flow(G, V_SOURCE, V_SINK);

	int profit = 0;
	for (unsigned i = 0; i < J; i++) {
		if (res_capacities[job_edges[i]] > 0) {
			profit += res_capacities[job_edges[i]];
		}
	}

	cout << profit << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
