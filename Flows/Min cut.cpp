//Returns edges (their numbers) that form the min cut
vector<int> get_min_cut(bool are_edges_directed) const {
    assert(flow_calculated);
    vector<char> us(V);
    auto dfs = [&](auto&& dfs, int v) -> void {
        us[v] = 1;
        for (int i : l[v]) {
            const auto& e = store[i];
            if (us[e.to]) continue;
            if (e.cap > 0) dfs(dfs, e.to);
        }
    };
    dfs(dfs, source);
    vector<int> res;
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
