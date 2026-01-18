class hopcroft_karp {
    int L, R, mt_size = 0;
    vector<vector<int>> l;
    vector<int> mtl, mtr;
    bool matching_calculated = false;

public:
    hopcroft_karp(int _L, int _R): L(_L), R(_R) {
        l.resize(L);
        mtl.resize(L, -1);
        mtr.resize(R, -1);
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
        assert(0 <= x && x < L);
        assert(0 <= y && y < R);
        l[x].push_back(y);
    }

    //Complexity: at most O(Esqrt(V))
    //Idea: do what Dinic algorithm does, just remove unnecessary parts of it
    vector<array<int, 2>> calc_max_matching() {
        vector<int> layer(L);
        auto bfs = [&]() {
            fill(layer.begin(), layer.end(), L);
            deque<int> dq;
            for (int v = 0; v < L; ++v) if (mtl[v] == -1) layer[v] = 0, dq.push_back(v);
            int fl = 0;
            while (dq.size()) {
                int v = dq.front(); dq.pop_front();
                for (int h : l[v]) {
                    if (mtr[h] == -1) fl = 1;
                    else if (layer[mtr[h]] == L) {
                        layer[mtr[h]] = layer[v] + 1;
                        dq.push_back(mtr[h]);
                    }
                }
            }
            return fl;
        };
        function<int(int)> dfs = [&](int v) {
            for (int h : l[v]) {
                if (mtr[h] == -1 || (layer[mtr[h]] == layer[v] + 1 && dfs(mtr[h]))) {
                    return mtr[h] = v, mtl[v] = h, 1;
                }
            }
            layer[v] = L;
            return 0;
        };
        while (bfs()) {
            for (int v = 0; v < L; ++v) {
                if (mtl[v] == -1) mt_size += dfs(v);
            }
        }
        vector<array<int, 2>> ans(mt_size);
        for (int v = 0, j = 0; v < L; ++v) {
            if (mtl[v] != -1) ans[j++] = {v, mtl[v]};
        }
        matching_calculated = true;
        return ans;
    }
};
