#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

using namespace std;
using namespace boost;

typedef	adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;
typedef	graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

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

void testcase() {
	int n, m;
	cin >> n >> m;

	vector<pair<int, int>> edges(m);
	vector<int> weights(m);
	for(int i = 0; i < m; i++) {
		cin >> edges[i].first >> edges[i].second >> weights[i];
	}
	int min_src = 0;
	int min_tar;
	int min_flow = INT_MAX;

	Graph G(n);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i = 0; i < m; i++) {
		eaG.addEdge(edges[i].first, edges[i].second, weights[i]);
	}

	for(int i = 0; i < n; i++) {
		int src = i, tar = (i+1)%n;

		long flow = push_relabel_max_flow(G, src, tar);

		if(flow < min_flow) {
			min_flow = flow;
			min_src = src;
			min_tar = tar;
		}
	}

	long flow = push_relabel_max_flow(G, min_src, min_tar);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);

	vector<int> vis(n, false); // visited flags
	std::queue<int> Q;
	int src = min_src;
	vis[src] = true;
	Q.push(src);
	int visited = 1;
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = target(*ebeg, G);
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			if(v < n) { visited += 1; }
			Q.push(v);
		}
	}

	cout << min_flow << endl;
	cout << visited << " ";
	for (int i = 0; i < n; ++i) {
		if (vis[i]) cout << i << " ";
	}
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) testcase();
	return 0;
}

