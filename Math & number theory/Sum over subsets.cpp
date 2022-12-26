template<typename T>
vector<T> sum_over_subsets(vector<T> m) {
    int n = m.size(), lg = __lg(geq_pow2(n));
    for (int q = 0; q < lg; q++) {
        for (int w = 0; w < n; ++w) {
            if (w >> q & 1) m[w] += m[w ^ (1 << q)];
        }
    }
    return m;
}