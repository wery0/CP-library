//High-precise (~10^-13) convolution.
//Replacing long double with double results faster (25-35%), but less precision (~10^-10).
template<typename U, typename T = long double>
vector<T> mul(const vector<U>& arr1, const vector<U>& arr2) {
	static_assert(is_floating_point_v<T>);
    if (arr1.empty() || arr2.empty()) return {};
    const T PI = 3.14159265358979323846264;
    const size_t res_size = arr1.size() + arr2.size() - 1;
    const size_t mx = max(arr1.size(), arr2.size());
    const size_t n = mx & (mx - 1) ? 2 << __lg(mx) : mx;
    vector<complex<T>> arr1c(n * 2), arr2c(n * 2);
    for (size_t i = 0; i < arr1.size(); ++i) arr1c[i] = arr1[i];
    for (size_t i = 0; i < arr2.size(); ++i) arr2c[i] = arr2[i];
    vector<int> perm(n * 2);
    auto build = [&](auto&& build, size_t l, size_t r, size_t d, size_t s) {
        if (l == r) {
            perm[l] = d;
            return;
        }
        size_t md = (l + r) >> 1;
        build(build, l, md, d, s << 1);
        build(build, md + 1, r, d + s, s << 1);
    };
    build(build, 0, n * 2 - 1, 0, 1);
    vector<complex<T>> pw(n);
    auto fft = [&](size_t n, vector<complex<T>>& m, bool inv) {
        for (size_t i = 0; i < n; ++i) {
            if (i < perm[i]) swap(m[i], m[perm[i]]);
        }
        for (size_t d = 2; d <= n; d <<= 1) {
            const size_t md = d >> 1;
            pw[0] = {1, 0};
            pw[1] = {cosl(2 * PI / d), (inv ? -1 : 1) * sinl(2 * PI / d)};
            for (size_t i = 2; i < md; ++i) pw[i] = pw[i - 1] * pw[1];
            for (size_t l = 0; l < n; l += md) {
                for (size_t i = 0; i < md; ++i, ++l) {
                    const complex<T> p = pw[i] * m[l + md];
                    m[l + md] = m[l] - p;
                    m[l] += p;
                }
            }
        }
    };
    fft(n * 2, arr1c, 0);
    fft(n * 2, arr2c, 0);
    for (size_t i = 0; i < n * 2; ++i) arr1c[i] *= arr2c[i];
    fft(n * 2, arr1c, 1);
    vector<T> ans(res_size);
    for (size_t i = 0; i < res_size; ++i) {
        if constexpr(is_integral_v<U>) ans[i] = roundl(arr1c[i].real() / (n * 2));
        else ans[i] = arr1c[i].real() / (n * 2);
    }
    return ans;
}
