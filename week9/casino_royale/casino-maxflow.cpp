#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

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
	int n, m, l;
	cin >> n >> m >> l;

	// sorted missions (by start), start -> (end, priority)
	// multimap<int, pair<int, int>> missions;
	// index is x (starting station), map is from end station to priority
	vector<vector<int>> stations(n);
	vector<vector<int>> missions(m);

	for (int i = 0; i < m; i++) {
		int x, y, p;
		cin >> x >> y >> p;
//		missions.insert(make_pair(x, make_pair(y, p)));
		missions[i].push_back(x);
		missions[i].push_back(y);
		missions[i].push_back(p);
		stations[x].push_back(i);
	}

	int source = 2*m;
	int source_ = 2*m+1;
	int sink = 2*m+2;

	// Build the graph
	Graph G(2*m+3);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
//    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	// Limit max. capacity
	eaG.addEdge(source, source_, l, 0);
	// Edges for each mission:
	for(vector<vector<int>>::iterator it = missions.begin(); it != missions.end(); ++it) {
		int i = it - missions.begin();
		int y = (*it)[1], p = (*it)[2];
		eaG.addEdge(source_, 2*i, 1, 0);
		eaG.addEdge(2*i, 2*i+1, 1, -p);
		eaG.addEdge(2*i+1, sink, 1, 0);
		// Now add edges to all following missions:
		for(vector<vector<int>>::iterator s_it = stations.begin() + y; s_it != stations.end(); ++s_it) {
			vector<int> s_missions = *s_it;
			for(vector<int>::iterator m_it = s_missions.begin(); m_it != s_missions.end(); ++m_it) {
				int j = *m_it;
				eaG.addEdge(2*i+1, 2*j, 1, 0);
			}
		}
	}
	// Maxflow-mincost
	push_relabel_max_flow(G, source, sink);
	cycle_canceling(G);
	int cost = find_flow_cost(G);
	// print out the readjusted cost (negative value)
	cout << -cost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
