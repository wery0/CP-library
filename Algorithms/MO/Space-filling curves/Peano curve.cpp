//Maps (x, y) -> position in peano curve
//O(log(max(x, y)))
template<typename T = uint64_t>
T peano_encode(T x, T y) {
    static_assert(is_unsigned_v<T>);
    T pw = 0;
    for (T s = 1; s <= max(x, y); ++pw) s *= 3;
    vector<T> f(pw), g(pw);
    for (int i = 0; i < pw; ++i) {
        f[pw - 1 - i] = x % 3;
        g[pw - 1 - i] = y % 3;
        x /= 3;
        y /= 3;
    }
    T p = 0;
    array<T, 2> sm = {0, 0};
    for (int i = 0; i < pw * 2; ++i) {
        T r = (i & 1 ? g : f)[i / 2];
        if (sm[~i & 1] & 1) r = 2 - r;
        sm[i & 1] += r;
        p = p * 3 + r;
    }
    return p;
}

//Maps position in peano curve -> (x, y)
//O(log(p))
template<typename T = uint64_t>
array<T, 2> peano_decode(T p) {
    static_assert(is_unsigned_v<T>);
    T pw = 0;
    for (T s = 1; s * s <= p; ++pw) s *= 3;
    vector<T> m(pw * 2);
    for (T i = 0; i < pw * 2; ++i, p /= 3) m[pw * 2 - 1 - i] = p % 3;
    T x = 0, y = 0;
    for (T i = 0, s0 = 0, s1 = 0; i < pw; ++i) {
        s0 += m[i * 2];
        x = x * 3 + (s1 & 1 ? 2 - m[i * 2] : m[i * 2]);
        y = y * 3 + (s0 & 1 ? 2 - m[i * 2 + 1] : m[i * 2 + 1]);
        s1 += m[i * 2 + 1];
    }
    return {x, y};
}
