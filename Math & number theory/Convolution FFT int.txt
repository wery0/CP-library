template<typename T>
vec<T> mul(vec<T> m, vec<T> n, const T mod) {

    auto binpow = [&](ll x, ll k) {
        ll o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };

    int res_size = m.size() + n.size() - 1;
    int a = geq_pow2(max(m.size(), n.size()));
    m.resize(a * 2);
    n.resize(a * 2);
    int B = -1;
    for (int q = 1; ; ++q) {
        if (binpow(q, (mod - 1) / 2) == mod - 1) {
            B = q;
            break;
        }
    }
    B = binpow(B, (mod - 1) / (a * 2));
    vec<int> perm(a * 2);

    auto build = [&](auto && build, int l, int r, int d, int s) {
        if (l == r) {
            perm[l] = d;
            return;
        }
        int md = (l + r) >> 1;
        build(build, l, md, d, s << 1);
        build(build, md + 1, r, d + s, s << 1);
    };
    build(build, 0, a * 2 - 1, 0, 1);

    vec<T> pw(a);
    auto fft = [&](int a, vec<T> &m, int t) {
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

    fft(a * 2, m, B);
    fft(a * 2, n, B);
    for (int q = 0; q < a * 2; q++) m[q] = 1ll * m[q] * n[q] % mod;
    fft(a * 2, m, binpow(B, mod - 2));
    const ll kek = binpow(a * 2, mod - 2);
    for (int q = 0; q < a * 2; q++) m[q] = m[q] * kek % mod;
    m.resize(res_size);
    return m;
}