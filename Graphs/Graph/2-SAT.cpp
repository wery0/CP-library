class two_sat {

    size_t V;

    size_t opp(size_t v) {
        return v < V ? v + V : v - V;
    }

    graph<edge<void>> g;

public:
    two_sat() = default;

    two_sat(size_t V, size_t D = 0): V(V), g(V * 2, D * 2) {}

    //Adds (x || y) restriction. If variable >= V, then it's negation of it.
    void add_disjunction(size_t x, size_t y) {
        assert(max(x, y) < V * 2);
        g.add_edge(opp(x), y, 1);
        g.add_edge(opp(y), x, 1);
    }

    void clear() {
        g.clear();
    }

    //Returns binary array: ans[i] = 0/1 if variable is false/true. Returns empty array if no answer.
    //O(V)
    vector<int> solve() {
        g.prepare();
        auto [nc, col] = g.calc_scc_coloring();
        for (size_t v = 0; v < V; ++v) {
            if (col[v] == col[opp(v)]) return {};
        }
        vector<int> ans(V);
        for (size_t v = 0; v < V; ++v) ans[v] = col[v] > col[opp(v)];
        return ans;
    }
};
