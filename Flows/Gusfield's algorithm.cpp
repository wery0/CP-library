/*
Gusfield's algorithm for finding Gomory-Hu tree
Complexity: n * O(flow(V, E))
Problems:
https://codeforces.com/contest/343/problem/E
*/
template<typename T>
class gusfield_gomory_hu {
    struct edge {
        int x, y; T w;
        edge(int x, int y, T w): x(x), y(y), w(w) {}
    };
    size_t V;
    vector<edge> edges;

public:
    gusfield_gomory_hu(size_t V): V(V) {
        assert(V >= 1);
    }

    void clear() {
        edges.clear();
    }

    //Edge is undirected
    void add_edge(int x, int y, T weight) {
        assert(0 <= min(x, y) && max(x, y) < V);
        if (x == y) return;
        edges.emplace_back(x, y, weight);
    }

    //Returns vector of edges of Gomory-Hu tree in form {x, y, weight}
    vector<array<T, 3>> calc_gomory_hu_tree() const {
        dinic<T> kek(V);
        for (auto [x, y, w] : edges) kek.add_edge(x, y, w, 0);
        vector<int> p(V);
        vector<array<T, 3>> res(V - 1);
        for (int s = 1; s < V; ++s) {
            int t = p[s];
            T f = kek.calc_max_flow(s, t, false);
            res[s - 1] = {s, t, f};
            for (auto v : kek.get_min_cut(0, false)) p[v] = p[v] == t ? s : p[v];
            kek.reset_flow();
        }
        return res;
    }
};
