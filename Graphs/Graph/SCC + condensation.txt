struct SCC {
private:
    void dfs2(int v, int col) {
        color[v] = col;
        for (const auto &h : inv[v]) {
            if (color[h.to] == -1) dfs2(h.to, col);
        }
    }

    void dfs1(int v) {
        us[v] = 1;
        for (const auto &h : l[v]) {
            if (!us[h.to]) dfs1(h.to);
        }
        top_sort.push_back(v);
    }

public:
    int V, ncolor = 0;
    graph<edge<void>> l, inv, g;
    vector<bool> us;
    vector<int> top_sort, color;

    SCC(int V, int E = -1) : V(V) {
        us.resize(V);
        color.resize(V, -1);
        l = graph<edge<void>>(V, E);
        inv = graph<edge<void>>(V, E);
    }

    void add_edge(int x, int y) {
        l.add_edge(x, y, 1);
        inv.add_edge(y, x, 1);
    }

    void calc() {
        l.prepare();
        inv.prepare();
        top_sort.reserve(V);
        for (int q = 0; q < V; q++) {
            if (!us[q]) dfs1(q);
        }
        reverse(all(top_sort));
        for (int v : top_sort) {
            if (color[v] == -1) dfs2(v, ncolor++);
        }
        g = graph<edge<void>>(ncolor);
        for (int q = 0; q < V; q++) {
            for (const auto &h : l[q]) {
                if (color[q] == color[h.to]) continue;
                g.add_edge(color[q], color[h.to], 1);
            }
        }
        unify(g.acc_edge);
        g.prepare();
    }
};
//Vertexes of condensation graph are numbered in order of topological sort