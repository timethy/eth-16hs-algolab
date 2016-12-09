#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long>>>>> Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap &capacitymap;
	EdgeWeightMap &weightmap;
	ReverseEdgeMap &revedgemap;

public:
	EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
			: G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

	Edge addEdge(int u, int v, long c, long w) {
		Edge e, reverseE;
		tie(e, tuples::ignore) = add_edge(u, v, G);
		tie(reverseE, tuples::ignore) = add_edge(v, u, G);
		capacitymap[e] = c;
		weightmap[e] = w;
		capacitymap[reverseE] = 0;
		weightmap[reverseE] = -w;
		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
		return e;
	}
};

void testcase() {
	int n, m, l;
	cin >> n >> m >> l;

	int v_source = n;
	int max_prio = 0;
	vector<int> xs(m), ys(m), ps(m);
	for (int i = 0; i < m; i++) {
		int x, y, p;
		cin >> x >> y >> p;
		xs[i] = x;
		ys[i] = y;
		ps[i] = p;
	}

	max_prio = *max_element(ps.begin(), ps.end());

	// Build the graph
	Graph G(n + 1);
	EdgeCapacityMap capacities = get(edge_capacity, G);
	EdgeWeightMap weights = get(edge_weight, G);
	ReverseEdgeMap rev = get(edge_reverse, G);
	ResidualCapacityMap res_capacities = get(edge_residual_capacity, G);
	EdgeAdder ea(G, capacities, weights, rev);

	map<Edge, int> length_map;
	// Limit max. capacity
	ea.addEdge(v_source, 0, l, 0);
	for (int i = 0; i < n - 1; i++) {
		Edge e = ea.addEdge(i, i + 1, l, max_prio);
		length_map.insert(make_pair(e, 1));
	}
	for (int i = 0; i < m; i++) {
		Edge e = ea.addEdge(xs[i], ys[i], 1, max_prio * (ys[i] - xs[i]) - ps[i]);
		length_map.insert(make_pair(e, ys[i] - xs[i]));
	}

	// Maxflow-mincost
	successive_shortest_path_nonnegative_weights(G, v_source, n - 1);
	long cost = 0;
	// Shift it by length*max_prio
	EdgeIt e, e_end;
	for (tie(e, e_end) = edges(G); e != e_end; ++e) {
		long flow = capacities[*e] - res_capacities[*e];
		if (flow == 1) {
			cost += flow * (weights[*e] - max_prio * length_map[*e]);
		}
	}
	cout << -cost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
