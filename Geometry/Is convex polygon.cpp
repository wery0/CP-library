template<typename T>
bool is_convex_polygon(const vector<pt<T>>& m) {
    int n = m.size(), sgn = 0;
    for (int q = 0; q < n; ++q) {
        pt<T> v1 = m[(q + 1) % n] - m[q];
        pt<T> v2 = m[(q + 2) % n] - m[(q + 1) % n];
        T val = sign(cross(v1, v2));
        if (val == 0) continue;
        if (sgn == 0) sgn = val;
        else if (sgn != val) return 0;
    }
    return 1;
}