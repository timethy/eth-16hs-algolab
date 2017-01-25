// for algolab, by taubnert@ethz.ch
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
using namespace boost;

typedef adjacency_list <vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2 <K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

typedef K::Point_2 P;

// If you're function has more than 8 variables, you screwed up!
inline pair<vector<bool>, int> safe_missions(int n, vector <P> &s, vector <P> &t,
                                  vector<P>& sp, vector<P>& ep,
                                  vector<unsigned>& jsp, vector<unsigned>& jep,
                                  vector<P>& j,
                                  vector<pair<int, int>> &edges, double omega,
                                             bool only_count) {
	Graph G(n);
	int a = 0, b = edges.size();
	while(a != b) {
		auto k = (a+b)/2;
		auto p = edges[k];
		if(CGAL::squared_distance(j[p.first], j[p.second]) <= omega) {
			a = k+1;
		} else {
			b = k;
		}
	}
	for(int i = 0; i < a; i++) {
		add_edge(edges[i].first, edges[i].second, G);
	}

	vector<int> comps(n);
	connected_components(G, make_iterator_property_map(comps.begin(), get(vertex_index, G)));

	vector<bool> safe;
	if(!only_count) {
		safe = vector<bool>(s.size(), false);
	}
	int num = 0;
	for (unsigned i = 0; i < s.size(); i++) {
		bool works = comps[jsp[i]] == comps[jep[i]] &&
		             (4*CGAL::squared_distance(s[i], sp[i]) <= omega) &&
		             (4*CGAL::squared_distance(t[i], ep[i]) <= omega);
		if(works) {
			num += 1;
			if(!only_count) {
				safe[i] = true;
			}
		}
	}
	return make_pair(safe, num);
}

void testcase() {
	unsigned long n, m;
	double p;
	cin >> n >> m >> p;

	vector<P> j(n);
	map<P, unsigned> pos_jammer;
	vector<P> s(m), t(m);
	vector<P> sp(m), ep(m);
	vector<unsigned> jsp(m), jep(m);

	for (unsigned i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		j[i] = P(x,y);
		pos_jammer.insert(make_pair(j[i], i));
	}

	Triangulation del;
	del.insert(j.begin(), j.end());
	for (unsigned i = 0; i < m; i++) {
		int x0, y0, x1, y1;
		cin >> x0 >> y0 >> x1 >> y1;
		s[i] = P(x0, y0);
		t[i] = P(x1, y1);
		sp[i] = del.nearest_vertex(s[i])->point();
		ep[i] = del.nearest_vertex(t[i])->point();
		jsp[i] = pos_jammer.at(sp[i]);
		jep[i] = pos_jammer.at(ep[i]);
	}

	vector<pair<int,int>> edges_by_distance;
	for (Edge_iterator e = del.finite_edges_begin(); e != del.finite_edges_end(); ++e) {
		auto source = del.segment(e).source();
		auto target = del.segment(e).target();
		edges_by_distance.push_back(make_pair(pos_jammer.at(source), pos_jammer.at(target)));
	}
	sort(edges_by_distance.begin(), edges_by_distance.end(), [&](pair<int, int> p1, pair<int, int> p2) {
		return CGAL::squared_distance(j[p1.first], j[p1.second]) <
				CGAL::squared_distance(j[p2.first], j[p2.second]);
	});

	vector<bool> safe;
	int num;
	tie(safe, num) = safe_missions(n, s, t, sp, ep, jsp, jep, j, edges_by_distance, p, false);
	for (unsigned i = 0; i < m; i++) {
		if (safe[i]) {
			cout << "y";
		} else {
			cout << "n";
		}
	}
	cout << endl;

	double min_p = 0;
	double min_all_p = 0;
	/*for (unsigned i = 0; i < m; i++) {
		auto min_start = 4*CGAL::squared_distance(s[i], sp[i]);
		auto min_end = 4*CGAL::squared_distance(t[i], ep[i]);
		min_all_p = max(min_all_p, max(min_start, min_end));
		if(safe[i]) {
			min_p = max(min_p, max(min_start, min_end));
		}
	}*/
	int n_a, n_b;
	double p_a, p_b;
	{
		// Now look for min power consumption for given missions
		int a = 0, b = edges_by_distance.size();
		while (a != b) {
			auto k = (a + b)/2;
			auto edge = edges_by_distance[k];
			auto k_p = CGAL::squared_distance(j[edge.first], j[edge.second]);
			if(k_p <= p) {
				vector<bool> safe_new;
				int num_new;
				tie(safe_new, num_new) = safe_missions(n, s, t, sp, ep, jsp, jep, j, edges_by_distance, k_p, true);
				if (num != num_new) {
					a = k + 1;
				} else {
					b = k;
				}
			} else {
				b = k;
			}
		}
		n_b = a;
		p_b = CGAL::squared_distance(j[edges_by_distance[n_b].first], j[edges_by_distance[n_b].second]);
	}
	{
		// Now look for a
		double a = max(p_b, min_all_p), b = max(p, min_all_p);
		double k;
		vector<bool> all_safe(m, true);
		vector<bool> safe_new;
		int num_new;
		tie(safe_new, num_new) = safe_missions(n, s, t, sp, ep, jsp, jep, j, edges_by_distance, b, true);
		while (num_new != m) {
			a = b+1;
			b = max(floor(b*2), b+1);
			tie(safe_new, num_new) = safe_missions(n, s, t, sp, ep, jsp, jep, j, edges_by_distance, b, true);
		}
		while (a != b) {
			k = floor((a + b)/2);
			tie(safe_new, num_new) = safe_missions(n, s, t, sp, ep, jsp, jep, j, edges_by_distance, k, true);
			if (m != num_new) {
				a = k + 1;
			} else {
				b = k;
			}
		}
		p_a = a;
	}
	cout << p_a << endl;
	cout << p_b << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	cout << fixed << setprecision(0);
	while (t--) { testcase(); }
	return 0;
}
