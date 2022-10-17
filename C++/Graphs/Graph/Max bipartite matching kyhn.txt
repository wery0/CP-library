struct MaxBipartiteMatchingKyhn {
private:
    int L, R;
    graph<edge<void>> g;
    vector<int> mtl,  mtr;
    vector<bool> us;
    bool sw = 0;

    int dfs(int v) {
        if (us[v]) return 0;
        us[v] = 1;
        for (const auto &h : g[v]) if (mtr[h.to] == -1) return mtr[h.to] = v, mtl[v] = h.to, 1;
        for (const auto &h : g[v]) if (dfs(mtr[h.to])) return mtr[h.to] = v, mtl[v] = h.to, 1;
        return 0;
    }

public:
    MaxBipartiteMatchingKyhn(int _L, int _R, int E = -1): L(_L), R(_R) {
        if (L > R) swap(L, R), sw = 1;
        g = graph<edge<void>>(L, E);
        mtl.resize(L, -1);
        mtr.resize(R, -1);
        us.resize(L);
    }

    void add_edge(int x, int y) {
        if (sw) swap(x, y);
        g.add_edge(x, y, 1);
    }

    vector<pair<int, int>> calc() {
        g.prepare();
        int mt_size = 0;
        for (;;) {
            fill(all(us), 0);
            int was = mt_size;
            for (int q = 0; q < L; ++q) {
                if (mtl[q] == -1) mt_size += dfs(q);
            }
            if (was == mt_size) break;
        }
        vector<pair<int, int>> ans(mt_size);
        for (int q = 0, i = 0; q < L; ++q) {
            if (mtl[q] != -1) {
                ans[i++] = {q, mtl[q]};
            }
        }
        if (sw) for (auto &p : ans) swap(p.F, p.S);
        return ans;
    }
};