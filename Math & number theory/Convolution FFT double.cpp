//High-precise (~10^-13) convolution.
//Replacing long double with double results faster (25-35%), but less precision (~10^-10).
template<typename T = long double>
vector<T> mul(vector<T> arr1, vector<T> arr2) {
    if (arr1.empty() || arr2.empty()) return {};
    const ld PI = 3.14159265358979323846264;
    int res_size = arr1.size() + arr2.size() - 1;
    int n = max(arr1.size(), arr2.size());
    n = n & (n - 1) ? 2 << __lg(n) : n;
    vector<complex<T>> arr1c(n * 2), arr2c(n * 2);
    for (int q = 0; q < arr1.size(); q++) arr1c[q] = arr1[q];
    for (int q = 0; q < arr2.size(); q++) arr2c[q] = arr2[q];
    vector<int> perm(n * 2);
    auto build = [&](auto&& build, int l, int r, int d, int s) {
        if (l == r) {
            perm[l] = d;
            return;
        }
        int md = (l + r) >> 1;
        build(build, l, md, d, s << 1);
        build(build, md + 1, r, d + s, s << 1);
    };
    build(build, 0, n * 2 - 1, 0, 1);
    vector<complex<T>> pw(n);
    auto fft = [&](int a, vector<complex<T>>& m, int inv) {
        for (int q = 0; q < a; q++) {
            if (q < perm[q]) swap(m[q], m[perm[q]]);
        }
        for (int d = 2; d <= a; d <<= 1) {
            const int md = d >> 1;
            pw[0] = {1, 0};
            pw[1] = {cosl(2 * PI / d), (inv ? -1 : 1) * sinl(2 * PI / d)};
            for (int q = 2; q < md; q++) pw[q] = pw[q - 1] * pw[1];
            for (int l = 0; l < a; l += md) {
                for (int q = 0; q < md; q++, l++) {
                    const complex<T> p = pw[q] * m[l + md];
                    m[l + md] = m[l] - p;
                    m[l] += p;
                }
            }
        }
    };
    fft(n * 2, arr1c, 0);
    fft(n * 2, arr2c, 0);
    for (int q = 0; q < n * 2; q++) arr1c[q] = arr1c[q] * arr2c[q];
    fft(n * 2, arr1c, 1);
    vector<T> ans(res_size);
    for (int q = 0; q < res_size; ++q) {
        ans[q] = roundl(arr1c[q].real() / (n * 2));
    }
    return ans;
}