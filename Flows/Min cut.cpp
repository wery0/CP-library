//Mode == 0: Returns reachable vertexes in residual network from source
//Mode == 1: Returns numbers of edges that form the min cut
vector<int> get_min_cut(int mode, bool are_edges_directed) const {
    assert(flow_calculated);
    assert(mode == 0 || mode == 1);
    vector<char> us(V);
    vector<int> res;
    function<void(int)> dfs = [&](int v) {
        us[v] = 1;
        if (mode == 0) res.push_back(v);
        for (int i : l[v]) {
            const auto& e = store[i];
            if (us[e.to]) continue;
            if (e.cap - e.flow > 0) dfs(e.to);
        }
    };
    dfs(source);
    if (mode == 0) return res;
    for (int v = 0; v < V; ++v) {
        if (!us[v]) continue;
        for (int i : l[v]) {
            const auto& e = store[i];
            if (!us[e.to] && (!are_edges_directed || (~i & 1))) {
                res.push_back(i / 2);
            }
        }
    }
    sort(res.begin(), res.end());
    return res;
}
