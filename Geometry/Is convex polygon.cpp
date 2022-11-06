template<typename T>
bool is_convex_polygon(vec<pt<T>> m) {
    int a = m.size(), sgn = 0;
    for (int q = 0; q < a; ++q) {
        pt<T> v1 = m[(q + 1) % a] - m[q];
        pt<T> v2 = m[(q + 2) % a] - m[(q + 1) % a];
        T val = sign(cross(v1, v2));
        if (val == 0) continue;
        if (sgn == 0) sgn = val;
        else if (sgn != val) return 0;
    }
    return 1;
}