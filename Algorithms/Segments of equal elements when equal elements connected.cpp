//Splits segment [L, R] into segments with equal values [l, r, val] and returns them as vector
//Constraint: equal values are connected, i. e. for every two equal values, all the values in between them should be the same as these values
//O(# f() calls) time = O(sum(log(r - l + 1))) <= R - L + 1
template<typename T>
auto get_segs_of_eq_elements_when_eq_connected(T L, T R, const auto& f) {
    using V = decltype(f(L));
    struct seg {T l, r; V v;};
    vector<seg> ans;
    assert(L <= R);
    if (L == R) return vector<seg>{{L, R, f(L)}};
    auto append = [&](T l, T r, V v) {
        if (ans.empty() || ans.back().v != v) ans.emplace_back(l, r, v);
        else ans.back().r = r;
    };
    function<void(T, T, V, V)> go = [&](T l, T r, V vl, V vr) {
        if (l > r) return;
        if (vl == vr) {append(l, r, vl); return;}
        if (l + 1 == r) {append(l, l, vl); append(r, r, vr); return;}
        T m = l + (r - l) / 2;
        V vm = f(m);
        go(l, m, vl, vm);
        if (l < m) go(m, r, vm, vr);
    };
    go(L, R, f(L), f(R));
    return ans;
}
