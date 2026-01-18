class max_bipartite_matching_kuhn {
    int L, R, tim = 0, mt_size = 0;
    vector<vector<int>> l;
    vector<int> mtl, mtr, us;
    bool sw = 0;
    bool matching_calculated = false;

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
    max_bipartite_matching_kuhn(int _L, int _R): L(_L), R(_R) {
        if (L > R) swap(L, R), sw = 1;
        l.resize(L);
        mtl.resize(L, -1);
        mtr.resize(R, -1);
        us.resize(L);
    }

    void clear() {
        fill(mtl.begin(), mtl.end(), -1);
        fill(mtr.begin(), mtr.end(), -1);
        mt_size = 0;
        for (auto& i : l) i.clear();
        matching_calculated = false;
    }

    void add_edge(int x, int y) {
        assert(!matching_calculated);
        if (sw) swap(x, y);
        assert(0 <= x && x < L);
        assert(0 <= y && y < R);
        l[x].push_back(y);
    }

    //Complexity: at most O(nm), but fast on practice
    //Idea: while augmenting path exists, find it using dfs and alternate edges in it
    vector<array<int, 2>> calc_max_matching() {
        while (true) {
            ++tim;
            int was = mt_size;
            for (int i = 0; i < L; ++i) {
                if (mtl[i] == -1) mt_size += dfs(i);
            }
            if (was == mt_size) break;
        }
        vector<array<int, 2>> ans(mt_size);
        for (int v = 0, j = 0; v < L; ++v) {
            if (mtl[v] != -1) ans[j++] = {v, mtl[v]};
        }
        if (sw) for (auto& [x, y] : ans) swap(x, y);
        matching_calculated = true;
        return ans;
    }

    pair<vector<int>, vector<int>> get_min_vertex_cover() {
        assert(matching_calculated);
        vector<vector<int>> nl(L + R);
        for (int v = 0; v < L; ++v) {
            for (int h : l[v]) {
                if (mtl[v] == h) nl[L + h].push_back(v);
                else nl[v].push_back(L + h);
            }
        }
        vector<char> us(L + R);
        function<void(int)> dfs = [&](int v) -> void {
            us[v] = 1;
            for (int h : nl[v]) {
                if (!us[h]) dfs(h);
            }
        };
        for (int v = 0; v < L; ++v) {
            if (mtl[v] == -1 && !us[v]) dfs(v);
        }
        vector<int> ml, mr;
        for (int v = 0; v < L; ++v) {
            if (!us[v]) ml.push_back(v);
        }
        for (int v = 0; v < R; ++v) {
            if (us[L + v]) mr.push_back(v);
        }
        if (sw) swap(ml, mr);
        return {ml, mr};
    }
};
