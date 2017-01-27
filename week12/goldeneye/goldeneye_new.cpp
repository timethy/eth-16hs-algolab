#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef K::Point_2 P;

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;

struct M {
	double d2_4; // 4 times the bigger squared distance (s to nearest jammer, t to nearest jammer);
	int js, jt;
	int idx;
};
struct E {
	double d2;
	int j1, j2;
};

inline int n_viable_missions(vector<E>& edges_by_distance, vector<M>& missions_by_distance, double omega) {
	int viable = 0;

	// find viable missions:
	M m_dummy;
	m_dummy.d2_4 = omega;
	auto ub_missions = upper_bound(missions_by_distance.begin(), missions_by_distance.end(), m_dummy, [](M m1, M m2) {
		return m1.d2_4 < m2.d2_4;
	});
	if(ub_missions == missions_by_distance.begin()) {
		return viable;
	}
	E e_dummy;
	e_dummy.d2 = omega;
	auto ub_edges = upper_bound(edges_by_distance.begin(), edges_by_distance.end(), e_dummy, [](E e1, E e2) {
		return e1.d2 < e2.d2;
	});

	// construct graph (all viable edges, i. e. edges with d2 < omega)
	int n = edges_by_distance.size();
	Graph G(n);
	vector<int> comps(n);
	for(auto it = edges_by_distance.begin(); it != ub_edges; it++) {
		auto edge = *it;
		add_edge(edge.j1, edge.j2, G);
	}

	connected_components(G, make_iterator_property_map(comps.begin(), get(vertex_index, G)));

	for(auto it = missions_by_distance.begin(); it != ub_missions; it++) {
		auto mission = *it;
		if(comps[mission.js] == comps[mission.jt]) {
			viable += 1;
		}
	}
	return viable;
}

inline vector<int> viable_missions(vector<E>& edges_by_distance, vector<M>& missions_by_distance, double omega) {
	vector<int> viable;

	// find viable missions:
	M m_dummy;
	m_dummy.d2_4 = omega;
	auto ub_missions = upper_bound(missions_by_distance.begin(), missions_by_distance.end(), m_dummy, [](M m1, M m2) {
		return m1.d2_4 < m2.d2_4;
	});
	if(ub_missions == missions_by_distance.begin()) {
		return viable;
	}
	E e_dummy;
	e_dummy.d2 = omega;
	auto ub_edges = upper_bound(edges_by_distance.begin(), edges_by_distance.end(), e_dummy, [](E e1, E e2) {
		return e1.d2 < e2.d2;
	});

	// construct graph (all viable edges, i. e. edges with d2 < omega)
	int n = edges_by_distance.size();
	Graph G(n);
	vector<int> comps(n);
	for(auto it = edges_by_distance.begin(); it != ub_edges; it++) {
		auto edge = *it;
		add_edge(edge.j1, edge.j2, G);
	}

	connected_components(G, make_iterator_property_map(comps.begin(), get(vertex_index, G)));

	for(auto it = missions_by_distance.begin(); it != ub_missions; it++) {
		auto mission = *it;
		if(comps[mission.js] == comps[mission.jt]) {
			viable.push_back(mission.idx);
		}
	}
	return viable;
}

void testcase() {
  // read number of points
  size_t n, m, p;
  cin >> n >> m >> p;

  vector<P> jammers(n);
  map<P, int> pos_for_jammer;
  vector<pair<P,P>> missions(m);

  for(unsigned i = 0; i < n; i++) {
	  int x, y;
	  cin >> x >> y;
	  jammers[i] = P(x,y);
	  //cin >> jammers[i];
	  pos_for_jammer[jammers[i]] = i;
  }

  Triangulation del;
  del.insert(jammers.begin(), jammers.end());

  vector<double> all_distances;
  vector<E> edges_by_distance;
  edges_by_distance.reserve(3*n);

  // Calculate edge distances:
  for (Edge_iterator e = del.finite_edges_begin(); e != del.finite_edges_end(); ++e) {
	  auto u = del.segment(e).source();
	  auto v = del.segment(e).target();
	  E edge;
	  edge.j1 = pos_for_jammer[u];
	  edge.j2 = pos_for_jammer[v];
	  edge.d2 = del.segment(e).squared_length();
	  edges_by_distance.push_back(edge);
	  all_distances.push_back(edge.d2);
  }

  vector<M> missions_by_distance(m);

  for(unsigned i = 0; i < m; i++) {
	  M &mission = missions_by_distance[i];
	  int x0, y0, x1, y1;
	  cin >> x0 >> y0 >> x1 >> y1;
	  auto p0 = P(x0, y0);
	  auto p1 = P(x1, y1);
	  //cin >> missions[i].first >> missions[i].second;
	  auto v_s = del.nearest_vertex(p0)->point(),
			  v_t = del.nearest_vertex(p1)->point();
	  double d_s2 = CGAL::squared_distance(v_s, p0),
			  d_t2 = CGAL::squared_distance(v_t, p1);
	  auto d = max(d_s2, d_t2);
	  mission.d2_4 = 4*d;
	  mission.js = pos_for_jammer[v_s];
	  mission.jt = pos_for_jammer[v_t];
	  mission.idx = i;
	  all_distances.push_back(mission.d2_4);
  }

  // sort both arrays by distances:
  sort(edges_by_distance.begin(), edges_by_distance.end(), [](E e1, E e2) {
	  return e1.d2 < e2.d2;
  });
  sort(missions_by_distance.begin(), missions_by_distance.end(), [](M m1, M m2) {
	  return m1.d2_4 < m2.d2_4;
  });

  vector<int> p_viable = viable_missions(edges_by_distance, missions_by_distance, p);
  vector<bool> mission_viable(m, false);

  for(const int &i : p_viable) {
	  mission_viable[i] = true;
  }
  for(const bool &b : mission_viable) {
	  cout << (b ? 'y' : 'n');
  }
  cout << endl;

  // sort all distances so we can do a binary search over it..
  sort(all_distances.begin(), all_distances.end());
  //unique(all_distances.begin(), all_distances.end());

  int lmin, lmax;
  // find a first
  lmin = 0;
  lmax = all_distances.size()-1;
  while(lmin != lmax) {
	  auto k = (lmin+lmax)/2;
	  auto omega = all_distances[k];
	  if(n_viable_missions(edges_by_distance, missions_by_distance, omega) < m) {
		  lmin = k+1;
	  } else {
		  lmax = k;
	  }
  }
  cout << all_distances[lmin] << endl;

  if(p_viable.size() == 0) {
	  cout << 0 << endl;
	  return;
  }
  // Now find b
  lmax = lmin;
  lmin = 0;
  while(lmin != lmax) {
	  auto k = (lmin+lmax)/2;
	  auto omega = all_distances[k];
	  if(n_viable_missions(edges_by_distance, missions_by_distance, omega) < p_viable.size()) {
		  lmin = k+1;
	  } else {
		  lmax = k;
	  }
  }
  cout << all_distances[lmin] << endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	cout << fixed << setprecision(0);
	while(t--) { testcase(); }
	return false;
}
