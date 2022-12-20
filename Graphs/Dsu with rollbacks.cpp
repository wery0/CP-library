struct dsu_w_rollbacks {
    int n;
    vector<int> pr;
    vector<int> sz;
    vector<pair<int, int>> store;

    dsu_w_rollbacks() = default;
    dsu_w_rollbacks(int n): n(n), sz(n, 1), pr(n) {
        iota(all(pr), 0);
    }

    void clear() {
        fill(all(sz), 1);
        iota(all(pr), 0);
        store.clear();
    }

    bool is_in_same_component(int x, int y) {return find(x) == find(y);}

    int get_cur_version() {return store.size();}

    int find(int x) {return x == pr[x] ? x : find(pr[x]);}

    int unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return 0;
        if (sz[px] > sz[py]) swap(px, py);
        store.emplace_back(px, py);
        pr[px] = py;
        sz[py] += sz[px];
        return 1;
    }

    void rollback() {
        auto [px, py] = store.back(); store.pop_back();
        pr[px] = px;
        sz[py] -= sz[px];
    }

    void revert_to_version(int version) {
        assert(0 <= version && version <= store.size());
        while (version < store.size()) rollback();
    }
};