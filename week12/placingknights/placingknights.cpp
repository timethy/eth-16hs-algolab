// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
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

inline int V_idx(int n, int i, int j) {
	return n*i+j;
}

void testcases() {
	int n;
	cin >> n;

	vector<vector<bool>> board(n, vector<bool>(n));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int b;
			cin >> b;
			board[i][j] = b == 1;
		}
	}

	auto V = n*n+2;
	Graph G(V);
	auto V_SOURCE = n*n;
	auto V_SINK = n*n+1;
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for (int i = 0; i < n; ++i) {
		// Go through all black tiles
		for (int j = 0; j < n; j++) {
			if(board[i][j] && (i+j)%2==0) {
				vector<pair<int, int>> points;
				points.push_back(make_pair(i-1,j-2));
				points.push_back(make_pair(i+1,j-2));
				points.push_back(make_pair(i-2,j-1));
				points.push_back(make_pair(i-2,j+1));
				points.push_back(make_pair(i-1,j+2));
				points.push_back(make_pair(i+1,j+2));
				points.push_back(make_pair(i+2,j-1));
				points.push_back(make_pair(i+2,j+1));
				for(auto &p: points) {
					if(p.first >= 0 && p.second >= 0 && p.first < n && p.second < n && board[i][j]) {
						eaG.addEdge(V_idx(n, i, j), V_idx(n, p.first, p.second), 1);
					}
				}
			}
		}
	}
	int fields = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; j++) {
			if(board[i][j]) {
				fields+=1;
				if((i+j)%2==0) {
					eaG.addEdge(V_SOURCE, V_idx(n,i,j), 1);
				} else {
					eaG.addEdge(V_idx(n,i,j), V_SINK, 1);
				}
			}
		}
	}

	long flow = push_relabel_max_flow(G, V_SOURCE, V_SINK);

	cout << fields-flow << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--)	testcases();
	return 0;
}



