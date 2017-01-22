// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
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
	unsigned n, m, s;
	cin >> n >> m >> s;

	Graph G(n+2);
	EdgeCapacityMap capacities = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	EdgeAdder ea(G, capacities, revedgemap);

	const auto V_SOURCE = n;
	const auto V_SINK = n + 1;

	vector<unsigned> sweepers(n);
	vector<unsigned> doors(n);
	vector<unsigned> degree(n);

	for (unsigned i = 0; i < s; i++) {
		unsigned start;
		cin >> start;
		sweepers[start]++;
		degree[start]++;
	}
	for (unsigned i = 0; i < s; i++) {
		unsigned end;
		cin >> end;
		doors[end]++;
		degree[end]++;
	}
	for (unsigned i = 0; i < m; i++) {
		unsigned u, v;
		cin >> u >> v;
		ea.addEdge(u, v, 1);
		ea.addEdge(v, u, 1);
		degree[u]++;
		degree[v]++;
	}
	if(m == 0) {
		cout << "yes" << endl;
		return;
	}
	vector<int> components(n+2);
	int ncc = connected_components(G, &components[0]);
	for (int i = 0; i < ncc; i++) {
		// if component has a sweeper or is the sink or source (j >= n)
		bool has_sweeper = false;
		// there exist one sweeper per connected component
		for (unsigned j = 0; j < n+2; ++j) {
//			cout << "j=" << j << "i=" << i << endl;
			if(components[j] == i && (j >= n || sweepers[j] > 0)) {
				has_sweeper = true;
			}
		}
		if(!has_sweeper) {
			cout << "no" << endl;
			return;
		}
	}

	// EULER, Check if euler tours are possible
	for (unsigned i = 0; i < n; i++) {
		if(degree[i] % 2 != 0) {
			cout << "no" << endl;
			return;
		}
		ea.addEdge(V_SOURCE, i, sweepers[i]);
		ea.addEdge(i, V_SINK, doors[i]);
	}

	unsigned flow = edmonds_karp_max_flow(G, V_SOURCE, V_SINK);
	if(flow != s) {
		cout << "no" << endl;
		return;
	}

	cout << "yes" << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
