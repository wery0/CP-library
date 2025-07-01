//Simple polygon - a polygon that doesn't intersect itself and has no holes.
template<typename T>
T calc_double_area_of_simple_polygon(const vector<pt<T>>& m) {
    if (m.empty()) return 0;
    T ans = 0;
    pt<T> pr = m.back();
    for (const auto& p : m) {
        ans += cross(pr, p);
        pr = p;
    }
    return abs(ans);
}
