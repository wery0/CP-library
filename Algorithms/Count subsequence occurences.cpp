//Counts # occurences of [f2, l2) as subsequence in [f1, l1)
//O(n * m)
template<typename T, typename I1, typename I2>
T subsequences_count(I1 f1, I1 l1, I2 f2, I2 l2) {
    const size_t n = l1 - f1, m = l2 - f2;
    vector<T> dp(m + 1);
    dp[0] = 1;
    for (; f1 != l1; ++f1) {
        const auto val = *f1;
        auto it = prev(l2);
        for (size_t j = m; j; --j, --it) {
            if (*it == val) {
                dp[j] += dp[j - 1];
            }
        }
    }
    return dp[m];
}
