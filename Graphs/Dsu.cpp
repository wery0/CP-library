//Uncomment commented lines, if you want to maintain set of vertexes for each component.
struct dsu {
    int n;
    vector<int> pr;
    //vector<vector<int>> vrt;

    dsu() = default;
    dsu(int n): n(n), pr(n, -1) {
        //vrt.resize(n); for (int i = 0; i < n; ++i) vrt[i] = {i};
    }

    void clear() {
        fill(pr.begin(), pr.end(), -1);
        //for (int i = 0; i < n; ++i) vrt[i] = {i}, vrt[i].shrink_to_fit();
    }

    int get_component_size(int x) {return -pr[find(x)];}
    int find(int x) {return pr[x] < 0 ? x : pr[x] = find(pr[x]);}
    bool are_in_same_component(int x, int y) {return find(x) == find(y);}
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;
        if (pr[px] < pr[py]) swap(px, py);
        pr[py] += pr[px];
        pr[px] = py;
        //for(int v : vrt[px]) vrt[py].push_back(v);
        //vrt[px].clear(); vrt[px].shrink_to_fit();
        return true;
    }
};
