//Ensure that the ans and the product of any two numbers fits in T
//Verification: https://atcoder.jp/contests/abc173/tasks/abc173_e
template<typename T>
T calc_max_product_of_k(vector<T> m, size_t k) {
    static_assert(is_signed_v<T>);
    if (m.empty() || k == 0) return 1;
    size_t n = m.size(), l = 0, r = n - 1; assert(0 < k && k <= n);
    sort(m.begin(), m.end());
    T ans = 1, sgn = 1;
    if (k % 2) ans = m[r], sgn = ans < 0 ? -1 : 1, --r, --k;
    for (; k; k -= 2) {
        T x = m[l] * m[l + 1], y = m[r] * m[r - 1];
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

//Ensure that the product of any two numbers fits in T
template<typename T>
T calc_max_product_of_k_mod(vector<T> m, size_t k, T mod) {
    static_assert(is_signed_v<T>);
    if (m.empty() || k == 0) return 1;
    size_t n = m.size(), l = 0, r = n - 1; assert(0 < k && k <= n);
    sort(m.begin(), m.end());
    T ans = 1, sgn = 1;
    if (k % 2) ans = m[r], sgn = ans < 0 ? -1 : 1, --r, --k;
    for (; k; k -= 2) {
        T x = m[l] * m[l + 1], y = m[r] * m[r - 1];
        if (x * sgn > y * sgn) {
            ans = x % mod * ans % mod;
            l += 2;
        } else {
            ans = y % mod * ans % mod;
            r -= 2;
        }
    }
    if (ans < 0) ans += mod;
    return ans;
}