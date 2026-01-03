//Works for directed networks
//Returns vector of paths from source to sink
vector<pair<T_flow, vector<int>>> get_flow_path_decomposition(bool as_vertex_nums) const {
    assert(flow_calculated);
    vector<pair<T_flow, vector<int>>> res;
    vector<int> us(V), ptr(V), egs;
    auto s = store;
    int us_iter = 0;
    auto dfs = [&](auto&& dfs, int v, T_flow min_flow = numeric_limits<T_flow>::max()) -> T_flow {
        if (v == sink) return min_flow;
        if (us[v] == us_iter) return 0;
        us[v] = us_iter;
        ptr[v] = 0;
        for (int cnt = 0; cnt < l[v].size(); ++cnt) {
            int i = l[v][ptr[v]++];
            if (ptr[v] == l[v].size()) ptr[v] = 0;
            if (i & 1) continue;
            auto& e = s[i];
            if (s[i ^ 1].cap == 0) continue;
            T_flow res = dfs(dfs, e.to, min(min_flow, s[i ^ 1].cap));
            if (res > 0) {
                egs.push_back(i / 2);
                e.cap += res;
                s[i ^ 1].cap -= res;
                return res;
            }
        }
        return 0;
    };
    while (true) {
        ++us_iter;
        egs.clear();
        T_flow tyt = dfs(dfs, source);
        if (!tyt) break;
        reverse(egs.begin(), egs.end());
        if (as_vertex_nums) {
            for (int& i : egs) i = store[i * 2 ^ 1].to;
            egs.push_back(sink);
        }
        res.emplace_back(tyt, egs);
    }
    return res;
}
