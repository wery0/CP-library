//Returns set of points of minimal size, generating CH, sorted in counter-clockwise order.
template<typename T>
vector<pt<T>> convex_hull(vector<pt<T>> m) {
    constexpr T inf = numeric_limits<T>::max();
    pt mnp = {inf, inf};
    for (auto &p : m) chmin(mnp, p);
    for (auto &p : m) p -= mnp;
    pt<T> origin = {0, 0};
    sort(all(m), [&](const auto & p1, const auto & p2) {
        T c = cross(p1, p2);
        return c ? c > 0 : mhdst(origin, p1) < mhdst(origin, p2);
    });
    vector<pt<T>> o;
    for (int q = 0; q < isz(m); ++q) {
        for (; isz(o) > 1;) {
            if (cross(o.back() - o[o.size() - 2], m[q] - o.back()) > 0) break;
            o.pop_back();
        }
        o.push_back(m[q]);
    }
    for (auto &p : o) p += mnp;
    return o;
};