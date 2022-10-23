struct MaxBipartiteMatchingKyhn {
private:
    int L, R;
    vector<vector<int>> l;
    vector<int> mtl, mtr;
    vec<bool> us;
    bool sw = 0;

    int dfs(int v) {
        if (us[v]) return 0;
        us[v] = 1;
        for (int h : l[v]) if (mtr[h] == -1) return mtr[h] = v, mtl[v] = h, 1;
        for (int h : l[v]) if (dfs(mtr[h])) return mtr[h] = v, mtl[v] = h, 1;
        return 0;
    }

public:
    MaxBipartiteMatchingKyhn(int _L, int _R): L(_L), R(_R) {
        if (L > R) swap(L, R), sw = 1;
        l.resize(L);
        mtl.resize(L, -1);
        mtr.resize(R, -1);
        us.resize(L);
    }

    void add_edge(int x, int y) {
        if (sw) swap(x, y);
        l[x].push_back(y);
    }

    vector<pair<int, int>> calc() {
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