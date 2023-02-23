template<typename T>
bool is_convex_polygon(const vector<pt<T>>& m) {
    const int n = m.size();
    int sgn = 0;
    for (int i = 0; i < n; ++i) {
        pt<T> v1 = m[(i + 1) % n] - m[i];
        pt<T> v2 = m[(i + 2) % n] - m[(i + 1) % n];
        T val = cross(v1, v2);
        val = val < 0 ? -1 : val > 0 ? 1 : 0;
        if (val == 0) continue;
        if (sgn == 0) sgn = val;
        else if (sgn != val) return 0;
    }
    return 1;
}