//<O(NM), O(min(N, M))>
template<typename I>
int longest_common_subsequence_length(I f1, I l1, I f2, I l2) {
    if (l1 - f1 < l2 - f2) swap(f1, f2), swap(l1, l2);
    const size_t N = l1 - f1, M = l2 - f2;
    vector<int> dp(M);
    dp[0] = *f1 == *f2;
    for (int w = 1; w < M; ++w) dp[w] = dp[w - 1] || *f1 == *(f2 + w);
    for (size_t q = 1; q < N; ++q) {
        int pr = dp[0];
        dp[0] = dp[0] || *(f1 + q) == *f2;
        for (size_t w = 1; w < M; ++w) {
            const int prr = dp[w];
            if (*(f1 + q) == *(f2 + w)) dp[w] = pr + 1;
            else dp[w] = max(dp[w], dp[w - 1]);
            pr = prr;
        }
    }
    return dp.back();
}