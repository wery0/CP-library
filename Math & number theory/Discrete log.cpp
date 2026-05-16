//Implementation of Shanks'es baby-step giant-step algorithm for solving equation a ^ x = b (mod m)
//Returns minimal x if it exists, -1 otherwise
//Complexity: <O(sqrt(m)), O(sqrt(m))>
template<typename T, typename T2>
T discrete_log_bsgs(T a, T b, T m) {
    static_assert(is_integral_v<T> && is_signed_v<T>);
    static_assert(is_integral_v<T2> && is_signed_v<T2>);
    static_assert(sizeof(T) * 2 == sizeof(T2));
    auto mulmod = [&](T x, T y) {return T2(x) * y % m;};
    assert(0 <= a && a < m);
    assert(0 <= b && b < m);
    T k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) return add;
        if (b % g) return -1;
        b /= g, m /= g, ++add;
        k = mulmod(k, a / g);
    }
    T n = sqrtl(m) + 1, an = 1;
    for (T k = n, aa = a; k; k >>= 1, aa = mulmod(aa, aa)) if (k & 1) an = mulmod(an, aa);
    unordered_map<T, T> mp(n * 2);
    for (T q = 0, cur = b; q <= n; ++q, cur = mulmod(cur, a)) mp[cur] = q;
    for (T p = 1, cur = k; p <= n; ++p) {
        cur = mulmod(cur, an);
        if (mp.count(cur)) return n * p - mp[cur] + add;
    }
    return -1;
}
