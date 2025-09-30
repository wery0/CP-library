//a[i] = i < n ? a[i] : sum(c[j] * a[i - 1 - j])
//Calculates a[k] in O(O(mul) * log(k)) where O(mul) is O(n ^ 2) in this implementation, could be optimized to O(nlog(n)) with fft
template<typename T>
T kth_value_of_recurrence_relation_smart(const vector<T>& a, const vector<T>& c, int64_t k) {
    const size_t n = c.size();
    assert(k >= 0);
    assert(a.size() >= n && n > 0);
    if (k < n) return a[k];

    auto mul = [&](const vector<T>& x, const vector<T>& y) {
        vector<T> o(x.size() + y.size() - 1);
        for (int i = 0; i < x.size(); ++i) {
            for (int j = 0; j < y.size(); ++j) {
                o[i + j] += x[i] * y[j];
            }
        }
        for (int i = o.size() - 1; i >= n; --i) {
            for (int j = 0; j < n; ++j) {
                o[i - j - 1] += o[i] * c[j];
            }
        }
        if (n < o.size()) o.resize(n);
        return o;
    };

    vector<T> x = {1};
    for (vector<T> a = n == 1 ? vector<T>{c[0]} : vector<T>{0, 1}; k; k /= 2) {
        if (k & 1) x = mul(x, a);
        a = mul(a, a);
    }
    x.resize(n);

    T res = 0;
    for (size_t i = 0; i < n; ++i) res += x[i] * a[i];
    return res;
}
