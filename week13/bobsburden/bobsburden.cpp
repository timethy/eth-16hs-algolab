#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int>> Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

inline int k_index(int i, int j) {
	int idx = i+i*(i+1)/2-(i-j);
	//cout << i << "," << j << "->" << idx << endl;
	return idx;
}

void testcase() {
	int k;
	cin >> k;

	int V = k*(k+1)/2;

	vector<vector<int>> v(k);

	for(int i = 0; i < k; i++) {
		for(int j = 0; j <= i; j++) {
			int v_ij;
			cin >> v_ij;
			v[i].push_back(v_ij);
		}
	}

	Graph G(V);
	WeightMap weightmap = get(edge_weight, G);
	Edge e;	bool success;
	for (int i = 0; i < k; i++) {
		for(int j = 0; j <= i; j++) {
			if(j < i) {
				tie(e, success) = add_edge(k_index(i,j), k_index(i,j+1), G);
				weightmap[e] = v[i][j];
				tie(e, success) = add_edge(k_index(i,j+1), k_index(i,j), G);
				weightmap[e] = v[i][j+1];
			}
			if(i < k-1) {
				tie(e, success) = add_edge(k_index(i,j), k_index(i+1,j), G);
				weightmap[e] = v[i][j];
				tie(e, success) = add_edge(k_index(i,j), k_index(i+1,j+1), G);
				weightmap[e] = v[i][j];
				tie(e, success) = add_edge(k_index(i+1,j), k_index(i,j), G);
				weightmap[e] = v[i+1][j];
				tie(e, success) = add_edge(k_index(i+1,j+1), k_index(i,j), G);
				weightmap[e] = v[i+1][j+1];
			}
		}
	}

	vector<Vertex> predmap;
	predmap = vector<Vertex>(V);
	vector<int> d11(V);
	dijkstra_shortest_paths(G, k_index(0,0),
			predecessor_map(make_iterator_property_map(
					predmap.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					d11.begin(), get(vertex_index, G))));

	predmap = vector<Vertex>(V);
	vector<int> dk1(V);
	dijkstra_shortest_paths(G, k_index(k-1,0),
			predecessor_map(make_iterator_property_map(
					predmap.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					dk1.begin(), get(vertex_index, G))));

	predmap = vector<Vertex>(V);
	vector<int> dkk(V);
	dijkstra_shortest_paths(G, k_index(k-1,k-1),
			predecessor_map(make_iterator_property_map(
					predmap.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					dkk.begin(), get(vertex_index, G))));

	int min_burden = INT_MAX;
	for (int i = 0; i < k; i++) {
		for(int j = 0; j <= i; j++) {
			int idx = k_index(i,j);
			min_burden = min(min_burden, v[i][j] + d11[idx] + dk1[idx] + dkk[idx]);
		}
	}

	cout << min_burden << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcase();
	return 0;
}


