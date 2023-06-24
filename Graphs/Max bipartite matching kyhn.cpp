class max_bipartite_matching_kyhn {
    int L, R, tim = 0;
    vector<vector<int>> l;
    vector<int> mtl, mtr, us;
    bool sw = 0;

    int dfs(int v) {
        if (us[v] == tim) return 0;
        us[v] = tim;
        for (int h : l[v]) if (mtr[h] == -1) return mtr[h] = v, mtl[v] = h, 1;
        //shuffle(l[v].begin(), l[v].end(), rnd);
        rotate(l[v].begin(), l[v].begin() + l[v].size() / 3, l[v].end());
        for (int h : l[v]) if (dfs(mtr[h])) return mtr[h] = v, mtl[v] = h, 1;
        return 0;
    }

public:
    max_bipartite_matching_kyhn(int _L, int _R): L(_L), R(_R) {
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