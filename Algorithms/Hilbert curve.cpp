
//Maps (x, y) -> position in hilbert curve
//O(log(max(x, y)))
template<typename T = uint64_t>
T hilbert_encode(T x, T y) {
    static_assert(is_unsigned_v<T>);
    const T logn = __lg(max(x, y) * 2 + 1) | 1;
    const T maxn = (T(1) << logn) - 1;
    T p = 0;
    for (T s = T(1) << (logn - 1); s; s >>= 1) {
        bool rx = x & s, ry = y & s;
        p = (p << 2) | (rx ? ry ? 2 : 1 : ry ? 3 : 0);
        if (!rx) {
            if (ry) x ^= maxn, y ^= maxn;
            swap(x, y);
        }
    }
    return p;
}

//Maps position in hilbert curve -> (x, y)
//O(log(p))
template<typename T = uint64_t>
array<T, 2> hilbert_decode(T p) {
    static_assert(is_unsigned_v<T>);
    T x = 0, y = 0, it = __lg(p * 4 + 1) / 2;
    array<int, 4> m = {0, 2, 3, 1};
    if (~it & 1) swap(m[1], m[3]);
    for (; it--; ) {
        T s = T(1) << it;
        int q = p >> it * 2;
        p &= (T(1) << it * 2) - 1;
        if (m[q] & 1) y += s;
        if (m[q] & 2) x += s;
        if (q == 0) swap(m[1], m[3]);
        if (q == 3) swap(m[0], m[2]);
    }
    return {x, y};
}
