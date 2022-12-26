//Returns minimal set of points, s. t. each segment [l, r] contains at least one point inside.
template<typename T>
vector<T> covering_segments_with_points(vector<pair<T, T>> m) {
    if (m.empty()) return {};
    int n = m.size();
    sort(all(m));
    vector<T> ans;
    T r = m[0].second;
    for (const auto& p : m) {
        if (p.F > r) ans.push_back(r), r = p.second;
        else chmin(r, p.second);
    }
    if (ans.empty() || ans.back() != r) ans.push_back(r);
    return ans;
}
