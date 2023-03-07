//Returns smallest subset of arr which generates convex hull, sorted in CCW order.
template<typename T>
vector<pt<T>> convex_hull(vector<pt<T>> arr) {
    static const T eps = is_integral_v<T> ? 0 : EPS;
    if (arr.empty()) return {};
    pt<T> mnp = *min_element(arr.begin(), arr.end());
    for (auto& p : arr) p -= mnp;
    sort(arr.begin(), arr.end(), [&](const pt<T>& p1, const pt<T>& p2) {
        T c = cross(p1, p2);
        return abs(c) > eps ? c > 0 : abs(p1.x) + abs(p1.y) < abs(p2.x) + abs(p2.y);
    });
    vector<pt<T>> ch;
    for (const auto& p : arr) {
        while (ch.size() > 1) {
            if (cross(ch.back() - ch[ch.size() - 2], p - ch.back()) > eps) break;
            ch.pop_back();
        }
        if (ch.empty() || mdist(p, ch.back()) > eps) ch.push_back(p);
    }
    for (auto& p : ch) p += mnp;
    return ch;
}