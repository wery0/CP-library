//1.5+ times faster, than __gcd.
int gcd(int x, int y) {
    if (!x || !y) return x ^ y;
    int shx = __builtin_ctz(x), shy = __builtin_ctz(y);
    int sh = min(shx, shy);
    y >>= shy;
    while (x) {
        x >>= shx;
        int df = y - x;
        shx = __builtin_ctz(df);
        y = min(x, y);
        x = abs(df);
    }
    return y << sh;
}

ll gcdll(ll x, ll y) {
    if (!x || !y) return x ^ y;
    int shx = __builtin_ctzll(x), shy = __builtin_ctzll(y);
    int sh = min(shx, shy);
    y >>= shy;
    while (x) {
        x >>= shx;
        ll df = y - x;
        shx = __builtin_ctzll(df);
        y = min(x, y);
        x = abs(df);
    }
    return y << sh;
}