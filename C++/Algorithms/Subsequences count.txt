template<typename I1, typename I2>
ull subsequences_count(I1 f1, I1 l1, I2 f2, I2 l2) {
    int a = l1 - f1, b = l2 - f2;
    vec<ull> dp(b + 1);
    dp[0] = 1;
    for (; f1 != l1; ++f1) {
        const auto val = *f1;
        auto it = f2;
        for (int w = 1; w <= b; ++w, ++it) {
            if (*it == val) {
                dp[w] += dp[w - 1];
            }
        }
    }
    return dp[b];
}