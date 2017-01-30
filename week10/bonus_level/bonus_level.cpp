#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cstdlib>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Namespaces
using namespace boost;
using namespace std;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;
// Interior Property Maps
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

void testcase() {
	int n;
	cin >> n;

    int a_max = 0;
	vector<vector<int>> A(n, vector<int>(n));
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			cin >> A[i][j];
			a_max = max(A[i][j], a_max);
		}
	}

	auto N = n*n;
	Graph G(2*N+2);
	int v_source = 2*N;
	int v_target = 2*N+1;
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			int idx = i*n+j;
			eaG.addEdge(2*idx, 2*idx+1, 1, a_max-A[i][j]);
			eaG.addEdge(2*idx, 2*idx+1, 1, a_max);
		}
	}
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			int idx = i*n+j;
			if(n-1 > j) {
				int idx_right = i*n+j+1;
				eaG.addEdge(2*idx+1, 2*idx_right, 1, 0);
			}
			if(n-1 > i) {
				int idx_down = (i+1)*n+j;
				eaG.addEdge(2*idx+1, 2*idx_down,  1, 0);
			}
		}
	}
	int idx_start = 0;
	int idx_end = (n-1)*n+n-1;
	eaG.addEdge(v_source, 2*idx_start, 2, 0);
	eaG.addEdge(2*idx_end+1, v_target, 2, 0);

	successive_shortest_path_nonnegative_weights(G, v_source, v_target);
	int cost = find_flow_cost(G);

	cout << 2*(2*n-1)*a_max - cost << endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) testcase();
	return 0;
}

