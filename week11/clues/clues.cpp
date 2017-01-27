#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

typedef CGAL::Point_2<K> P;

using namespace std;
using namespace boost;

// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef	graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

void testcase()
{
	unsigned n, m;
	long r;
	cin >> n >> m >> r;

	long r2 = r*r;

	vector<P> stations(n);
	map<P, int> point_to_station;
	for(unsigned i = 0; i < n; i++) {
		cin >> stations[i];
		point_to_station[stations[i]] = i;
	}
	vector<pair<P,P>> clues(m);
	for(unsigned i = 0; i < m; i++) {
		cin >> clues[i].first >> clues[i].second;
	}

	Triangulation t;
	t.insert(stations.begin(), stations.end());

	/*
	for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		P p0 = f->vertex(0)->point(), p1 = f->vertex(1)->point(), p2 = f->vertex(2)->point();
		if(CGAL::squared_distance(p0, p1) <= r2) {
			if(CGAL::squared_distance(p0, p2) <= r2 ||
					CGAL::squared_distance(p1, p2) <= r2) {
				interferences = true;
				break;
			}
		}
	}*/
	// two coloring?
	Graph G(n);
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		auto p0 = e->first->vertex((e->second + 1) % 3)->point();
		auto p1 = e->first->vertex((e->second + 2) % 3)->point();
		if(CGAL::squared_distance(p0, p1) <= r2) {
			add_edge(point_to_station[p0], point_to_station[p1], G);
		}
	}
	bool bipartite = true;
	vector<P> L, R;
	vector<int> col(n, 0); // colors +-1, 0 means unvisited
	std::queue<int> Q;
	col[0] = -1;
	L.push_back(stations[0]);
	Q.push(0);
	int v_idx = 0;
	while (!Q.empty()) {
		const int u = Q.front();
		assert(col[u] != 0);
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = target(*ebeg, G);
			if (col[u] == col[v]) {
				bipartite = false;
				break;
			} else if(col[u] == -col[v]) { continue; }
			col[v] = -col[u];
			if(col[v] == -1) {
				L.push_back(stations[v]);
			} else {
				R.push_back(stations[v]);
			}
			Q.push(v);
		}
		if(Q.empty()) {
			while(v_idx < n && col[v_idx] != 0) { v_idx++; }
			if(v_idx < n) {
				col[v_idx] = -1;
				L.push_back(stations[v_idx]);
				Q.push(v_idx);
			}
		}
	}
	bool interference = false;
	if(bipartite) {
		Triangulation t_L;
		t_L.insert(L.begin(), L.end());
		for (Edge_iterator e = t_L.finite_edges_begin(); e != t_L.finite_edges_end(); ++e) {
			auto p0 = e->first->vertex((e->second + 1) % 3)->point();
			auto p1 = e->first->vertex((e->second + 2) % 3)->point();
			if(CGAL::squared_distance(p0, p1) <= r2) {
				interference = true;
				break;
			}
		}

		Triangulation t_R;
		t_R.insert(R.begin(), R.end());
		for (Edge_iterator e = t_R.finite_edges_begin(); e != t_R.finite_edges_end(); ++e) {
			auto p0 = e->first->vertex((e->second + 1) % 3)->point();
			auto p1 = e->first->vertex((e->second + 2) % 3)->point();
			if(CGAL::squared_distance(p0, p1) <= r2) {
				interference = true;
				break;
			}
		}
	}

	if(bipartite && !interference) {
		// connected components:
		vector<int> comp(n);
		connected_components(G, make_iterator_property_map(comp.begin(), get(vertex_index, G)));
		for(unsigned i = 0; i < m; i++) {
			if(CGAL::squared_distance(clues[i].first, clues[i].second) <= r2) {
				cout << "y";
			} else {
				auto nearest_a = t.nearest_vertex(clues[i].first)->point();
				auto nearest_b = t.nearest_vertex(clues[i].second)->point();
				//cout << nearest_a << " and " << nearest_b << endl;
				if(CGAL::squared_distance(nearest_a, clues[i].first) <= r2 &&
						CGAL::squared_distance(nearest_b, clues[i].second) <= r2 &&
						comp[point_to_station[nearest_a]] == comp[point_to_station[nearest_b]]) {
					cout << "y";
				} else {
					cout << "n";
				}
			}
		}
	} else {
		for(unsigned i = 0; i < m; i++) {
			cout << "n";
		}
	}
	cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcase();
	return 0;
}
