//Standart convolution of two arrays. O(nlogn).
//(mod - 1) should be divisible on 2^k, s. t. n <= 2^k.
//For example mod = 998244353 is almost always good, since 998244352 = 7 * 17 * 2^23
template<typename T>
vector<T> mul(vector<T> arr1, vector<T> arr2, const T mod) {
    if (arr1.empty() || arr2.empty()) return {};
    auto binpow = [&](ll x, ll k) {
        ll o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    int res_size = arr1.size() + arr2.size() - 1;
    int n = max(arr1.size(), arr2.size());
    n = n & (n - 1) ? 2 << __lg(n) : n;
    arr1.resize(n * 2);
    arr2.resize(n * 2);
    int B = -1;
    for (int q = 1; ; ++q) {
        if (binpow(q, (mod - 1) / 2) == mod - 1) {
            B = q;
            break;
        }
    }
    B = binpow(B, (mod - 1) / (n * 2));
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
    vector<T> pw(n);
    auto fft = [&](int a, vector<T>& m, int t) {
        for (int q = 0; q < a; q++) {
            if (q < perm[q]) swap(m[q], m[perm[q]]);
        }
        for (int d = 2; d <= a; d <<= 1) {
            const int md = d >> 1;
            pw[0] = 1;
            pw[1] = binpow(t, a / d);
            for (int q = 2; q < md; q++) pw[q] = 1ll * pw[q - 1] * pw[1] % mod;
            for (int l = 0; l < a; l += md) {
                for (int q = 0; q < md; q++, l++) {
                    const T u = 1ll * pw[q] * m[l + md] % mod;
                    m[l + md] = m[l] + mod - u;
                    m[l] += u;
                }
            }
            for (int q = 0; q < a; q++) m[q] -= m[q] < mod ? 0 : mod;
        }
    };
    fft(n * 2, arr1, B);
    fft(n * 2, arr2, B);
    for (int q = 0; q < n * 2; q++) arr1[q] = 1ll * arr1[q] * arr2[q] % mod;
    fft(n * 2, arr1, binpow(B, mod - 2));
    const ll kek = binpow(n * 2, mod - 2);
    for (int q = 0; q < n * 2; q++) arr1[q] = arr1[q] * kek % mod;
    arr1.resize(res_size);
    return arr1;
}