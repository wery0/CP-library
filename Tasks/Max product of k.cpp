//Ensure that the ans fits in T
//Verification: https://atcoder.jp/contests/abc173/tasks/abc173_e
template<typename T>
T calc_max_product_of_k(vec<T> m, int k) {
    int a = isz(m); assert(0 < k && k <= a);
    sort(all(m));
    ll l = 0, r = a - 1, ans = 1, sgn = 1;
    if (k % 2) ans = m[r], --r, --k, sgn = ans < 0 ? -1 : 1;
    for (; k; k -= 2) {
        ll x = m[l] * m[l + 1], y = m[r] * m[r - 1];
        if (x * sgn > y * sgn) {
            ans = x * ans;
            l += 2;
        } else {
            ans = y * ans;
            r -= 2;
        }
    }
    return ans;
}

template<typename T>
T calc_max_product_of_k_mod(vec<T> m, int k, const T M) {
    int a = isz(m); assert(0 < k && k <= a);
    sort(all(m));
    ll l = 0, r = a - 1, ans = 1, sgn = 1;
    if (k % 2) ans = m[r], --r, --k, sgn = ans < 0 ? -1 : 1;
    for (; k; k -= 2) {
        ll x = m[l] * m[l + 1], y = m[r] * m[r - 1];
        if (x * sgn > y * sgn) {
            ans = x % M * ans % M;
            l += 2;
        } else {
            ans = y % M * ans % M;
            r -= 2;
        }
    }
    if (ans < 0) ans += M;
    return ans;
}