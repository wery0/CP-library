//1.5+ times faster, than __gcd.
template<typename T>
T binary_gcd(T x, T y) {
    static_assert(is_integral_v<T> && is_signed_v<T>);
    if (!x || !y) return x ^ y;
    int shx = __builtin_ctzll(x), shy = __builtin_ctzll(y);
    int sh = min(shx, shy);
    y >>= shy;
    while (x) {
        x >>= shx;
        T df = y - x;
        shx = __builtin_ctzll(df);
        y = min(x, y);
        x = abs(df);
    }
    return y << sh;
}
