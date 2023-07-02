//Returns minimal set of points, s. t. each segment [l, r] contains at least one point inside.
//O(nlog(n))
template<typename T>
vector<T> covering_segments_with_points(vector<pair<T, T>> m) {
    if (m.empty()) return {};
    sort(m.begin(), m.end());
    vector<T> ans;
    T r = m[0].second;
    for (const auto& [lf, rg] : m) {
        if (lf > r) ans.push_back(r), r = rg;
        else r = min(r, rg);
    }
    if (ans.empty() || ans.back() != r) ans.push_back(r);
    return ans;
}