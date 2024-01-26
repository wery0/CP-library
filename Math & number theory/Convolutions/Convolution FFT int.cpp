//Standart convolution of two arrays. O(nlogn).
//mod must be prime and (mod - 1) should be divisible on 2^k, s. t. n <= 2^k.
//For example mod = 998244353 is almost always good, since 998244352 = 7 * 17 * 2^23
template<typename T>
vector<T> mul(vector<T> arr1, vector<T> arr2, const T mod) {
    if (arr1.empty() || arr2.empty()) return {};
    auto binpow = [&](int64_t x, int64_t k) {
        int64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    const size_t res_size = arr1.size() + arr2.size() - 1;
    size_t n = max(arr1.size(), arr2.size());
    n = n & (n - 1) ? 2 << __lg(n) : n;
    arr1.resize(n * 2);
    arr2.resize(n * 2);
    int64_t generator = -1;
    for (int64_t i = 1; ; ++i) {
        if (binpow(i, (mod - 1) / 2) == mod - 1) {
            generator = i;
            break;
        }
    }
    generator = binpow(generator, (mod - 1) / (n * 2));
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
    vector<T> pw(n);
    auto fft = [&](int n, vector<T>& m, int t) {
        for (size_t i = 0; i < n; ++i) {
            if (i < perm[i]) swap(m[i], m[perm[i]]);
        }
        for (size_t d = 2; d <= n; d <<= 1) {
            const size_t md = d >> 1;
            pw[0] = 1;
            pw[1] = binpow(t, n / d);
            for (size_t i = 2; i < md; ++i) pw[i] = 1ll * pw[i - 1] * pw[1] % mod;
            for (size_t l = 0; l < n; l += md) {
                for (size_t i = 0; i < md; ++i, ++l) {
                    const T u = 1ll * pw[i] * m[l + md] % mod;
                    m[l + md] = m[l] + mod - u;
                    m[l] += u;
                }
            }
            for (size_t i = 0; i < n; ++i) m[i] -= m[i] < mod ? 0 : mod;
        }
    };
    fft(n * 2, arr1, generator);
    fft(n * 2, arr2, generator);
    for (size_t i = 0; i < n * 2; ++i) arr1[i] = 1ll * arr1[i] * arr2[i] % mod;
    fft(n * 2, arr1, binpow(generator, mod - 2));
    const int64_t kek = binpow(n * 2, mod - 2);
    for (auto& val : arr1) val = val * kek % mod;
    arr1.resize(res_size);
    return arr1;
}
