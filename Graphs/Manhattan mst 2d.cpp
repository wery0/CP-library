//Returns {weight of mst, edges of mst} for the set of 2d points
//O(nlogn)
template<typename T, typename C>
pair<C, vector<array<int, 2>>> manhattan_mst_2d(vector<array<T, 2>> m) {
    vector<tuple<T, int, int>> candidates;
    candidates.reserve(m.size() * 4);
    vector<int> id(m.size()); iota(id.begin(), id.end(), 0);
    for (int rot = 0; rot < 4; ++rot) {
        sort(id.begin(), id.end(), [&](const auto& l, const auto& r) {return m[l][0] + m[l][1] < m[r][0] + m[r][1];});
        map<int, int, greater<int>> active;
        for (auto i : id) {
            for (auto it = active.lower_bound(m[i][0]); it != active.end(); active.erase(it++)) {
                int j = it->second;
                if (m[i][0] - m[i][1] > m[j][0] - m[j][1]) break;
                candidates.push_back({abs(m[i][0] - m[j][0]) + abs(m[i][1] - m[j][1]), i, j});
            }
            active[m[i][0]] = i;
        }
        for (auto& p : m) rot & 1 ? void(p[0] *= -1) : swap(p[0], p[1]);
    }
    sort(candidates.begin(), candidates.end());
    vector<int> pr(m.size(), -1);
    function<int(int)> find = [&](int x) {return pr[x] < 0 ? x : pr[x] = find(pr[x]);};
    auto unite = [&](int x, int y) {
        if (x = find(x), y = find(y); x == y) return false;
        if (pr[x] < pr[y]) swap(x, y);
        pr[y] += pr[x], pr[x] = y;
        return true;
    };
    C mst_weight = 0;
    vector<array<int, 2>> mst_edges(m.size() - 1);
    for (int i = 0; auto [w, u, v] : candidates) {
        if (unite(u, v)) {
            mst_weight += w;
            mst_edges[i++] = {u, v};
        }
    }
    return {mst_weight, mst_edges};
}
