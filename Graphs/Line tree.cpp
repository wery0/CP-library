//For a given edge-weighted tree builds a line tree s. t. min (max) on the path (u, v) in the original tree == min (max) on the path (u, v) in line tree for every (u, v)
//Returns {permutation of initial vertexes, edge weights between adjacent vertexes}
//Complexity: <O(nlog(n) for sorting + n * alpha), O(n)>
template<typename T>
pair<vector<int>, vector<T>> build_line_tree(vector<array<T, 3>> edges, bool is_minimum) {
    const size_t n = edges.size() + 1;
    sort(edges.begin(), edges.end(), [&](const auto& l, const auto& r){return l[2] < r[2];});
    if (is_minimum) reverse(edges.begin(), edges.end());
    vector<int> pr(n), sz(n, 1); iota(pr.begin(), pr.end(), 0);
    function<int(int)> find = [&](int v) {return pr[v] = pr[v] == v ? v : find(pr[v]);};
    vector<int> fir(n), lst(n), nxt(n, -1), p(n); iota(fir.begin(), fir.end(), 0); iota(lst.begin(), lst.end(), 0);
    vector<T> nw(n), w(n - 1);
    for (auto [x, y, w] : edges) {
        assert(0 <= x && x < n);
        assert(0 <= y && y < n);
        x = find(x), y = find(y);
        assert(x != y && "Input is not a tree u dumbass!");
        if (sz[x] > sz[y]) swap(x, y);
        pr[x] = y, sz[y] += sz[x];
        nxt[lst[y]] = fir[x];
        nw[lst[y]] = w;
        lst[y] = lst[x];
    }
    int root = find(0);
    assert(sz[root] == n && "Input is not a tree u dumbass!");
    for (int i = 0, v = fir[root]; i < n; ++i, v = nxt[v]) {
        p[i] = v;
        if (i != n - 1) w[i] = nw[v];
    }
    return {p, w};
}
