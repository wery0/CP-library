//a[i] = i < n ? a[i] : sum(c[j] * a[i - 1 - j])
//Calculates a[k] in min(O(n ^ 3 * log(k)), O((k - n + 1) * n))
template<typename T>
T kth_value_of_recurrence_relation_maxrix_exp(const vector<T>& a, const vector<T>& c, int64_t k) {
    const size_t n = c.size();
    assert(k >= 0);
    assert(a.size() >= n && n > 0);
    if (k < a.size()) return a[k];
    if (k < n * n * __lg(k)) {
        vector<T> m = a;
        for (int64_t i = n; i <= k; ++i) {
            T nw = 0;
            for (size_t j = 0; j < n; ++j) {
                nw += c[j] * m[n - 1 - j];
            }
            m.push_back(nw);
            m.erase(m.begin());
        }
        return m.back();
    }
    Matrix<T> row(1, n), mat(n, n);
    for (size_t i = 0; i < n; ++i) {
        row[0][i] = a[i];
        if (i + 1 < n) mat[i + 1][i] = 1;
        else {
            for (size_t j = 0; j < n; ++j) {
                mat[j][n - 1] = c[n - 1 - j];
            }
        }
    }
    mat.self_pow(k - n + 1);
    auto res = row * mat;
    return res[0][n - 1];
}
