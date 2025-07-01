//Returns all points of arr lying on convex hull, sorted in CCW order.
template<typename T>
vector<pt<T>> maximal_convex_hull(vector<pt<T>> arr) {
    static const T eps = is_integral_v<T> ? 0 : EPS;
    if (arr.empty()) return {};
    auto pis = [&](const pt<T>& p1, const pt<T>& p2, const pt<T>& p) -> bool {
        return abs(cross(p1 - p, p2 - p)) <= eps && dot(p2 - p1, p - p1) >= -eps && dot(p1 - p2, p - p2) >= -eps;
    };
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
    size_t d = 0;
    for (size_t rg = arr.size(); rg >= 1 && pis(ch.back(), ch[0], arr[rg - 1]);) --rg, ++d;
    vector<pt<T>> mxch;
    for (size_t j = 0, i = 1; j + d < arr.size(); ++j) {
        const auto& p = arr[j];
        while (cross(ch[i] - ch[0], p - ch[0]) > eps) ++i;
        if (pis(ch[i - 1], ch[i], p)) mxch.push_back(p);
    }
    mxch.insert(mxch.end(), arr.rbegin(), arr.rbegin() + d);
    for (auto& p : mxch) p += mnp;
    return mxch;
}
