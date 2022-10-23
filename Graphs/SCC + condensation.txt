struct SCC {
private:
    void dfs2(int v, int col) {
        color[v] = col;
        for (int h : inv[v]) if (color[h] == -1) dfs2(h, col);
    }

    void dfs1(int v) {
        us[v] = 1;
        for (int h : l[v]) if (!us[h]) dfs1(h);
        top_sort.push_back(v);
    }

public:
    int n, ncolor;
    vector<vector<int>> l, inv, g;
    vector<int> us, top_sort, color;

    SCC(int _n): n(_n) {
        l.resize(n);
        inv.resize(n);
        us.resize(n);
        color.resize(n, -1);
    }

    void add_edge(int x, int y) {
        l[x].push_back(y);
        inv[y].push_back(x);
    }

    void calc() {
        for (int q = 0; q < n; q++) {
            if (!us[q]) dfs1(q);
        }
        reverse(all(top_sort));
        for (int v : top_sort) {
            if (color[v] == -1) dfs2(v, ncolor++);
        }
        g.resize(ncolor);
        for (int q = 0; q < n; q++) {
            for (int h : l[q]) {
                if (color[q] == color[h]) continue;
                g[color[q]].push_back(color[h]);
            }
        }
        for (int q = 0; q < ncolor; q++) {
            sort(all(g[q]));
            g[q].erase(unique(all(g[q])), g[q].end());
        }
    }
};
//Vertexes of condensation graph are numered in order of topological sort