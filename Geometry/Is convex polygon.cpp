template<typename T>
bool is_convex_polygon(const vector<pt<T>>& m, bool strictly = false) {
    const T eps = is_integral_v<T> ? 0 : EPS;
    const size_t n = m.size();
    for (size_t i = 0, sgn = 0; i < n; ++i) {
        pt<T> v1 = m[(i + 1) % n] - m[i];
        pt<T> v2 = m[(i + 2) % n] - m[(i + 1) % n];
        T val = cross(v1, v2);
        val = val < -eps ? 2 : val > eps ? 1 : 0;
        if (val == 0 && strictly) return false;
        if (sgn == 0) sgn = val;
        else if (val && sgn != val) return false;
    }
    return true;
}