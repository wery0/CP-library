//<O(NM), O(min(N, M))>
template<typename I>
int calc_levenshtein_distance(I f1, I l1, I f2, I l2) {
    if (l1 - f1 < l2 - f2) swap(f1, f2), swap(l1, l2);
    const size_t N = l1 - f1, M = l2 - f2;
    vector<int> dp(M + 1);
    iota(dp.begin(), dp.end(), 0);
    for (size_t i = 1; i <= N; ++i) {
        int pr = dp[0];
        dp[0] = i;
        for (size_t j = 1; j <= M; ++j) {
            int ppr = dp[j];
            dp[j] = min({dp[j] + 1, dp[j - 1] + 1, pr + (*(f1 + i - 1) != *(f2 + j - 1))});
            pr = ppr;
        }
    }
    return dp[M];
}
