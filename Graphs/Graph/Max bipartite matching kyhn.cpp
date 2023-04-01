class MaxBipartiteMatchingKyhn {
    int L, R, tim = 0;
    graph<edge<void>> g;
    vector<int> mtl, mtr, us;
    bool sw = 0;

    int dfs(int v) {
        if (us[v] == tim) return 0;
        us[v] = tim;
        for (const auto& [h] : g[v]) if (mtr[h] == -1) return mtr[h] = v, mtl[v] = h, 1;
        rotate(g[v].begin(), g[v].begin() + g[v].size() / 3, g[v].end());
        //shuffle(g[v].begin(), g[v].end(), rnd);
        for (const auto& [h] : g[v]) if (dfs(mtr[h])) return mtr[h] = v, mtl[v] = h, 1;
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
        while (true) {
            ++tim;
            int was = mt_size;
            for (int i = 0; i < L; ++i) {
                if (mtl[i] == -1) mt_size += dfs(i);
            }
            if (was == mt_size) break;
        }
        vector<pair<int, int>> ans;
        ans.reserve(mt_size);
        for (int i = 0; i < L; ++i) {
            if (mtl[i] != -1) ans.emplace_back(i, mtl[i]);
        }
        if (sw) for (auto& [x, y] : ans) swap(x, y);
        return ans;
    }
};