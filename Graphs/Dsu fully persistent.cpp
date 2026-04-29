struct fully_persistent_dsu {
    int n;
    fully_persistent_array<int> pr;
    vector<int> vrs;

    fully_persistent_dsu() = default;
    fully_persistent_dsu(int n): n(n), pr(n, -1), vrs(1, 0) {}

    int find(int version, int x) const {
        int prx = pr(vrs[version], x);
        return prx < 0 ? x : find(version, prx);
    }

    bool are_in_same_component(int version, int x, int y) {
        vrs.push_back(pr.get_cur_version());
        return find(version, x) == find(version, y);
    }

    int unite(int version, int x, int y) {
        int px = find(version, x);
        int py = find(version, y);
        if (px == py) {vrs.push_back(pr.get_cur_version()); return 0;}
        int szx = pr(vrs[version], px), szy = pr(vrs[version], py);
        if (szx < szy) swap(px, py);
        pr.set_value(vrs[version], py, szx + szy);
        pr.set_value(pr.get_cur_version(), px, py);
        vrs.push_back(pr.get_cur_version());
        return 1;
    }
};
