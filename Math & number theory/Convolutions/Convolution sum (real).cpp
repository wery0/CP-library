//High-precise (~10^-13) sum convolution (aka polynomial product, fft). 
//Replacing D with double results faster (25-35%), but less precision (~10^-10).
//res[k] = sum({arr1[i] * arr2[j] | i + j == k})
//O(nlog(n))
template<typename T, typename D = long double>
vector<D> sum_convolution(const vector<T>& arr1, const vector<T>& arr2) {
    static_assert(is_floating_point_v<D>);
    if (arr1.empty() || arr2.empty()) return {};
    static constexpr D PI = 3.14159265358979323846264;
    const size_t res_size = arr1.size() + arr2.size() - 1;
    const size_t m = max(arr1.size(), arr2.size()) * 2;
    const size_t n = m & (m - 1) ? 2 << __lg(m) : m;
    vector<complex<D>> arr1c(n), arr2c(n);
    copy(arr1.begin(), arr1.end(), arr1c.begin());
    copy(arr2.begin(), arr2.end(), arr2c.begin());
    vector<int> perm(n);
    auto build = [&](auto&& build, size_t l, size_t r, size_t d, size_t s) {
        if (l == r) {perm[l] = d; return;}
        size_t md = (l + r) >> 1;
        build(build, l, md, d, s << 1);
        build(build, md + 1, r, d + s, s << 1);
    };
    build(build, 0, n - 1, 0, 1);
    vector<complex<D>> pw(n, {1, 0});
    auto fft = [&](vector<complex<D>>& m, bool inv) {
        for (size_t i = 0; i < n; ++i) {
            if (i < perm[i]) swap(m[i], m[perm[i]]);
        }
        for (size_t d = 1; d * 2 <= n; d <<= 1) {
            const complex<D> base = {cosl(PI / d), (inv ? -1 : 1) * sinl(PI / d)};
            for (size_t i = 1; i < d; ++i) pw[i] = pw[i - 1] * base;
            for (size_t l = 0; l < n; l += d) {
                for (size_t i = 0; i < d; ++i, ++l) {
                    const complex<D> p = pw[i] * m[l + d];
                    m[l + d] = m[l] - p;
                    m[l] += p;
                }
            }
        }
    };
    fft(arr1c, 0);
    fft(arr2c, 0);
    for (size_t i = 0; i < n; ++i) arr1c[i] *= arr2c[i];
    fft(arr1c, 1);
    vector<D> res(res_size);
    for (size_t i = 0; i < res_size; ++i) {
        if constexpr(is_integral_v<T>) res[i] = roundl(arr1c[i].real() / n);
        else res[i] = arr1c[i].real() / n;
    }
    return res;
}
