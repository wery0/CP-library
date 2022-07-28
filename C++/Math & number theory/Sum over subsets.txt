template<typename T>
vec<T> sum_over_subsets(vec<T> m, const T mod) {
    int a = m.size();
    int lg = __lg(geq_pow2(a));
    for (int q = 0; q < lg; q++) {
        for (int w = 0; w < a; ++w) {
            if (w >> q & 1) m[w] += m[w ^ (1 << q)];
        }
        for (int q = 0; q < a; ++q) m[q] -= m[q] < mod ? 0 : mod;
    }
    return m;
}