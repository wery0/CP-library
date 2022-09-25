template<typename T>
vec<T> covering_segments_with_points(vec<pair<T, T>> m) {
    int a = m.size();
    if (a == 0) return {};
    sort(all(m));
    vec<T> ans;
    T r = m[0].S;
    for (const auto &p : m) {
        if (p.F > r) ans.pb(r), r = p.S;
        else chmin(r, p.S);
    }
    if (ans.empty() || ans.back() != r) ans.pb(r);
    return ans;
}
//returns minimal set of points, s. t. each segment contains at least one point inside (including borders).