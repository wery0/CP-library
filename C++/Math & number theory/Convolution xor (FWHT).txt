ll inv(ll x, ll m) {return x == 1 ? x : (1 - inv(m % x, x) * m) / x + m;}

template<typename T>
vec<T> xor_convolution(vec<T> m, vec<T> n, const ll mod) {
    int a = geq_pow2(max(m.size(), n.size()));
    m.resize(a);
    n.resize(a);

    auto FWHT = [&](int a, vec<T> &m) {
        for (int d = 2; d <= a; d <<= 1) {
            const int md = d >> 1;
            for (int l = 0; l < a; l += md) {
                for (int q = 0; q < md; ++q, ++l) {
                    const T x = m[l + md];
                    m[l + md] = m[l] + mod - x;
                    m[l] += x;
                }
            }
            for (int q = 0; q < a; ++q) m[q] -= m[q] < mod ? 0 : mod;
        }
    };

    FWHT(a, m);
    FWHT(a, n);
    for (int q = 0; q < a; ++q) m[q] = 1ll * m[q] * n[q] % mod;
    FWHT(a, m);

    const ll ia = inv(a, mod);
    for (int q = 0; q < a; q++) m[q] = m[q] * ia % mod;
    return m;
}