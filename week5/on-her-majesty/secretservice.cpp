#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,
		property<edge_weight_t, int> > Graph;
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> UGraph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int exp_bin_search(int a, int s, int c, int d,
                   multimap<int, pair<int, int>>& a_to_s,
                   map<pair<int, int>, int>& a_to_s_,
                   int l, int r) {
	if(l == r) {
		return exp_bin_search(a, s, c, d, a_to_s, a_to_s_, r, 2*r);
	} else {
		int k = (l + r + 1)/2; // ceil
		// Build Graph
		UGraph H(a + c*s);
		for(multimap<int, pair<int, int>>::iterator it = a_to_s.begin(); it != a_to_s.upper_bound(k); ++it) {
			int alpha, omega;
			tie(alpha, omega) = it->second;
			add_edge(alpha, a + c*omega, H);
			if(c == 2 && a_to_s_[make_pair(alpha, omega)] + d <= k) {
				add_edge(alpha, a + c*omega + 1, H);
			}
		}
		vector<Vertex> matemap(a+c*s);
		edmonds_maximum_cardinality_matching(H, make_iterator_property_map(matemap.begin(), get(vertex_index, H)));
		int match_size = matching_size(H, make_iterator_property_map(matemap.begin(), get(vertex_index, H)));
		if(a <= match_size) {
			if(l + 1 == k) {
				return k;
			} else {
				return exp_bin_search(a, s, c, d, a_to_s, a_to_s_, l, k);
			}
		} else {
			return exp_bin_search(a, s, c, d, a_to_s, a_to_s_, k, r);
		}
	}
}

void testcase() {
	int n, m, a, s, c, d;
	cin >> n >> m >> a >> s >> c >> d;

	vector<int> as(a);
	vector<int> ss(s);
	int V = n+s;
	Graph G(V);
	for(int i = 0; i < m; i++) {
		char w;
		int x, y, z;
		cin >> w >> x >> y >> z;
		if(w == 'S') {
			add_edge(x, y, z, G);
		} else {
			add_edge(x, y, z, G);
			add_edge(y, x, z, G);
		}
	}
	for(int i = 0; i < a; i++) {
		int alpha;
		cin >> alpha;
		as[i] = alpha;
	}
	for(int i = 0; i < s; i++) {
		int omega;
		cin >> omega;
		ss[i] = omega;
		add_edge(omega, n+i, d, G);
	}

	multimap<int, pair<int, int>> a_to_s;
	map<pair<int, int>, int> a_to_s_;
	// Now compute shortest path for every agent to every shelter:
	for(int i = 0; i < a; i++) {
		vector<Vertex> predecessors(V);
		vector<int> distances(V);
		dijkstra_shortest_paths(G, as[i],
		                        predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, G))).
				                        distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, G))));
		for(int j = 0; j < s; j++) {
			a_to_s.insert(make_pair(distances[n+j], make_pair(i, j)));
			a_to_s_.insert(make_pair(make_pair(i, j), distances[n+j]));
		}
	}
	cout << exp_bin_search(a, s, c, d, a_to_s, a_to_s_, 0, 2) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) { testcase(); }
	return 0;
}

