struct dsu_w_rollbacks {
    int a;
    vec<int> pr;
    vec<int> sz;
    vec<pii> store;

    dsu_w_rollbacks() = default;

    dsu_w_rollbacks(int n): a(n) {
        sz.resize(a, 1);
        pr.resize(a);
        iota(all(pr), 0);
    }

    void clear() {
        fill(all(sz), 1);
        iota(all(pr), 0);
        store.clear();
    }

    bool in_same_component(int x, int y) {return find(x) == find(y);}

    int get_cur_version() {return store.size();}

    int find(int x) {return x == pr[x] ? x : find(pr[x]);}

    int unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return 0;
        if (sz[px] > sz[py]) swap(px, py);
        store.pb({px, py});
        pr[px] = py;
        sz[py] += sz[px];
        return 1;
    }

    void rollback() {
        pii p = store.back(); store.pop_back();
        pr[p.F] = p.F;
        sz[p.S] -= sz[p.F];
    }

    void revert_to_version(int version) {
        assert(0 <= version && version <= store.size());
        while (version < store.size()) rollback();
    }
};