// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

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
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::edge_iterator              EdgeIt;

class EdgeAdder {
    Graph& G;
    EdgeCapacityMap& capacitymap;
    EdgeWeightMap& weightmap;
    ReverseEdgeMap& revedgemap;

public:
    EdgeAdder(Graph& G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
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
	unsigned N, S;
	cin >> N >> S;

	// read in everything
	vector<int> l(S);
	for (unsigned i = 0; i < S; i++) {
		cin >> l[i];
	}
	vector<int> s(N);
	vector<int> t(N);
	vector<int> d(N);
	vector<int> a(N);
	vector<int> p(N);
	vector<set<int>> time(S);
	int max_p = 0;
	int max_t = 0;
	for (unsigned i = 0; i < N; i++) {
		cin >> s[i] >> t[i] >> d[i] >> a[i] >> p[i];
		time[s[i]-1].insert(d[i]);
		time[t[i]-1].insert(a[i]);
		max_p = max(max_p, p[i]);
		max_t = max(max_t, a[i]);
	}

	// For every station create the nodes
	map<pair<unsigned, int>, int> node_idx;
	int v_idx = 0;
	for (unsigned i = 0; i < S; i++) {
		for (set<int>::iterator it = time[i].begin();
		     it != time[i].end(); ++it) {
			node_idx.insert(make_pair(make_pair(i, *it), v_idx));
			v_idx += 1;
		}
	}
	// Two more nodes for the sink
	const int v_source = v_idx;
	const int v_sink = v_idx + 1;
	// MAX > MAX_BOOKINGS + MAX_INITIAL_CAPACITY
	const int MAX = 10000+100+1;

	Graph G(v_idx + 2);
    EdgeCapacityMap capacities = get(edge_capacity, G);
    EdgeWeightMap weights = get(edge_weight, G);
    ReverseEdgeMap rev = get(edge_reverse, G);
	ResidualCapacityMap res_capacities = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacities, weights, rev);

	map<Edge, int> length_map;
	// Add edge of capacity 1, priority -p_i for every booking
	for (unsigned i = 0; i < N; i++) {
		int duration = a[i] - d[i];
		Edge e = ea.addEdge(node_idx[make_pair(s[i]-1, d[i])],
		                    node_idx[make_pair(t[i]-1, a[i])], 1, max_p*duration-p[i]);
		length_map.insert(make_pair(e, duration));
	}
	for (unsigned i = 0; i < S; i++) {
		// Initial flow is l_i
		ea.addEdge(v_source, node_idx[make_pair(i, *time[i].begin())], l[i], 0);
		// Add edge of "infinite" capacity and priority 0 for cars staying
		for (set<int>::iterator it = time[i].begin();
		    it != time[i].end(); ++it) {
			set<int>::iterator it_next = it;
			it_next++;
			if(it_next == time[i].end()) {
				int duration = max_t - *it;
				// last station give infinite capacity edge to sink
				Edge e = ea.addEdge(node_idx[make_pair(i, *it)], v_sink, MAX, max_p*duration);
				length_map.insert(make_pair(e, duration));
			} else {
				int duration = *it_next - *it;
				Edge e = ea.addEdge(node_idx[make_pair(i, *it)],
				                    node_idx[make_pair(i, *it_next)], MAX, max_p*duration);
				length_map.insert(make_pair(e, duration));
			}
		}
	}

	// Mincost - Maxflow
	successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
	long cost = 0;
	// Shift it by length*max_p
	EdgeIt e, e_end;
	for(tie(e, e_end) = edges(G); e != e_end; ++e) {
		long flow = capacities[*e] - res_capacities[*e];
		if(flow == 1) {
			cost += flow * (weights[*e] - max_p * length_map[*e]);
		}
	}

	cout << -cost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}
