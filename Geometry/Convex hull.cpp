//Returns set of points of minimal size, generating CH, sorted in CCW order.
template<typename T>
vector<pt<T>> convex_hull(vector<pt<T>> m) {
    pt<T> mnp = *min_element(all(m));
    for (auto& p : m) p -= mnp;
    pt<T> origin = {0, 0};
    sort(all(m), [&](const auto& p1, const auto& p2) {
        T c = cross(p1, p2);
        return c ? c > 0 : mhdst(origin, p1) < mhdst(origin, p2);
    });
    vector<pt<T>> ch;
    for (int q = 0; q < m.size(); ++q) {
        for (; ch.size() > 1;) {
            if (cross(ch.back() - ch[ch.size() - 2], m[q] - ch.back()) > 0) break;
            ch.pop_back();
        }
        ch.push_back(m[q]);
    }
    for (auto& p : ch) p += mnp;
    return ch;
};