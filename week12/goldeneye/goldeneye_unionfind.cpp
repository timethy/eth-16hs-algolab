#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2 <K> Triangulation;

typedef K::Point_2 P;
typedef pair<K::Point_2, int> IPoint;

struct UnionFind {
	int n;
	vector<int> parent;
	vector<int> rank;

	UnionFind(int n_) : n(n_), parent(n_, -1), rank(n_, 1) {};

	int id(int elem) {
		// Find root:
		int i = elem;
		while (parent[i] >= 0) {
			i = parent[i];
		}
		return i;
	};

	// Union-by-rank
	void unite(int elem1, int elem2) {
		// traverse to root
		int i = id(elem1), j = id(elem2);
		if (i != j) {
			// compare rank:
			if (rank[i] <= rank[j]) {
				parent[i] = j;
				rank[j] += rank[i];
			} else {
				parent[j] = i;
				rank[i] += rank[j];
			}
		}
	};
};

struct Edge {
	Edge(int u_, int v_, K::FT d2_) : u(u_), v(v_), d2(d2_) {}
	int u, v;
	K::FT d2;
};

void testcase() {
	size_t n, m;
	double p;
	cin >> n >> m >> p;

// read jammers and build Delaunay
	vector <P> jammers;
	map <P, size_t> jammers_idx;
	jammers.reserve(n);
	for (size_t i = 0; i < n; ++i) {
		int x, y;
		cin >> x >> y;
		auto jammer = P(x, y);
		jammers.push_back(jammer);
		jammers_idx[jammer] = i;
	}
	Triangulation t;
	t.insert(jammers.begin(), jammers.end());

// extract edges and sort by length
	vector <Edge> edges;
	edges.reserve(3*n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		auto u = t.segment(e).source(), v = t.segment(e).target();
		edges.push_back(Edge(jammers_idx[u], jammers_idx[v], t.segment(e).squared_length()));
	}
	sort(edges.begin(), edges.end(), [](const Edge &e, const Edge &f) {
		return e.d2 < f.d2;
	});

// compute components with power consumption p
	UnionFind ufp(n);
	for (auto e = edges.begin(); e != edges.end() && e->d2 <= p; ++e) {
		ufp.unite(e->u, e->v);
	}

// handle missions
	K::FT a = 0;
	K::FT b = 0;
	UnionFind ufa(n);
	UnionFind ufb(n);
	auto it_a = edges.begin();
	auto it_b = edges.begin();
	for (size_t i = 0; i < m; ++i) {
		int x0, y0, x1, y1;
		cin >> x0 >> y0 >> x1 >> y1;
		P p0(x0, y0), p1(x1, y1);
		auto v0 = t.nearest_vertex(p0)->point();
		auto v1 = t.nearest_vertex(p1)->point();
		auto j0 = jammers_idx[v0], j1 = jammers_idx[v1];
		K::FT d = 4*max(CGAL::squared_distance(p0, v0),
		                CGAL::squared_distance(p1, v1));
		if (d <= p && ufp.id(j0) == ufp.id(j1)) {
			// mission possible with power p -> also with b
			cout << "y";
			if (d > b) {
				b = d;
			}
			// Add edges in ascending order (by length) until Union-Find stucture b contains both
			while (it_b != edges.end() && ufb.id(j0) != ufb.id(j1)) {
				ufb.unite(it_b->u, it_b->v);
				it_b++;
			}
		} else {
			cout << "n";
		}

		// ensure it is possible at power a
		if (d > a) {
			a = d;
		}
		while(it_a != edges.end() && ufa.id(j0) != ufa.id(j1)) {
			ufa.unite(it_a->u, it_a->v);
			it_a ++;
		}
	}
	cout << endl;
	// Check if it was limited by the edges or by the missions
	if (it_a != edges.begin() && (it_a - 1)->d2 > a) {
		 a = (it_a - 1)->d2;
	}
	if (it_b != edges.begin() && (it_b - 1)->d2 > b) {
		b = (it_b - 1)->d2;
	}
	cout << a << endl;
	cout << b << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cout << fixed << setprecision(0);
	size_t t;
	cin >> t;
	while (t--) { testcase(); }
	return 0;
}
