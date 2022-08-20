struct dsu {
    int a;
    vec<int> pr;
    vec<int> sz;
    //vec<vec<int>> vrt;

    dsu() {}

    dsu(int x) {
        a = x;
        sz = vec<int>(a, 1);
        pr = vec<int>(a);
        //vrt.resize(a); for (int q = 0; q < a; ++q) vrt[q] = {q};
        iota(all(pr), 0);
    }

    void clear() {
        fill(all(sz), 1);
        iota(all(pr), 0);
    }

    bool in_same_component(int x, int y) {return find(x) == find(y);}

    int find(int x) { return x == pr[x] ? x : pr[x] = find(pr[x]); }

    int unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return 0;
        if (sz[px] > sz[py]) swap(px, py);
        pr[px] = py;
        sz[py] += sz[px];
        //for(int v : vrt[px]) vrt[py].pb(v); vrt[px].clear(); vrt[px].shrink_to_fit();
        return 1;
    }
};