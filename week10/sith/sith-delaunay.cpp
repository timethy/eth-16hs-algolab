#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

using namespace std;
using namespace boost;

// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor Edge;	// Edge type

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Segment Segment;

int max_connected_component(map<K::Point_2, int>& points, vector<K::Point_2>& planets, int l, double r_squared) {
	size_t V = planets.size() - l;
	// Build the delaunay triangulation
	Triangulation t;
	t.insert(planets.begin() + l, planets.end());
	// Construct Graph
    Graph G(V);
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		auto source = t.segment(e).source();
		auto target = t.segment(e).target();
		if (CGAL::squared_distance(source, target) <= r_squared) {
			add_edge(points.at(source) - l, points.at(target) - l, G);
		}
	}
	// Find connected components
	vector<int> comps(V);
	int nscc = connected_components(G, make_iterator_property_map(comps.begin(), get(vertex_index, G)));
	// Get biggest connected component
	int m = 0;
	for (int i = 0; i < nscc; i++) {
		int m_ = count(comps.begin(), comps.end(), i);
		m = max(m, m_);
	}
	return m;
}

void testcase(int t) {
	int n, r;
	cin >> n >> r;

	map<K::Point_2, int> points;
	vector<K::Point_2> planets(n);
	
	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		planets[i] = K::Point_2(x, y);
		points.insert(make_pair(K::Point_2(x, y), i));
	}
	
	// Binary search
	int kl = 0; // the first feasible
	int kr = n/2+1; // the first unfeasible
	double r_squared = r;
	r_squared *= r_squared;
	while(kr - kl > 1) {
		int k = kl+(kr-kl)/2;
		int m = max_connected_component(points, planets, k, r_squared);
		if(k <= m) {
			kl = k;
		} else {
			kr = k;
		}
	}

	cout << kl << endl;
}

int main (void) {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		testcase(i);
	}
}

