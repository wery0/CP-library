struct dsu_w_rollbacks {
    int n;
    vector<int> pr;
    vector<pair<int, int>> store;

    dsu_w_rollbacks() = default;
    dsu_w_rollbacks(int n): n(n), pr(n, -1) {}

    void clear() {
        fill(all(pr), -1);
        store.clear();
    }

    int get_component_size(int x) const {return -pr[find(x)];}
    int find(int x) const {return pr[x] < 0 ? x : find(pr[x]);}
    int get_cur_version() const {return store.size();}
    bool is_in_same_component(int x, int y) const {return find(x) == find(y);}
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;
        if (pr[px] < pr[py]) swap(px, py);
        store.emplace_back(px, pr[px]);
        pr[py] += pr[px];
        pr[px] = py;
        return true;
    }

    void rollback() {
        auto [px, prpx] = store.back(); store.pop_back();
        pr[pr[px]] -= prpx;
        pr[px] = -1;
    }

    void revert_to_version(int version) {
        assert(0 <= version && version <= store.size());
        while (version < store.size()) rollback();
    }
};