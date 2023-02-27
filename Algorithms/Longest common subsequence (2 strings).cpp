//<O(NM), O(NM)>
template<typename I>
auto longest_common_subsequence(I f1, I l1, I f2, I l2) {
    using T = typename iterator_traits<I>::value_type;
    const size_t N = l1 - f1, M = l2 - f2;
    vector<vector<int>> dp(N + 1, vector<int>(M + 1));
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = 1; j <= M; ++j) {
            if (*(f1 + i - 1) == *(f2 + j - 1)) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    vector<T> ans(dp[N][M]);
    for (size_t i = N, j = M, pos = dp[N][M]; i && j; ) {
        if (*(f1 + i - 1) == *(f2 + j - 1)) {
            ans[--pos] = *(f1 + i - 1);
            --i, --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) --i;
        else --j;
    }
    return ans;
}

//<O(NM), O(min(N, M))>
template<typename I>
int longest_common_subsequence_length(I f1, I l1, I f2, I l2) {
    if (l1 - f1 < l2 - f2) swap(f1, f2), swap(l1, l2);
    const size_t N = l1 - f1, M = l2 - f2;
    vector<int> dp(M);
    dp[0] = *f1 == *f2;
    for (size_t j = 1; j < M; ++j) dp[j] = dp[j - 1] || *f1 == *(f2 + j);
    for (size_t i = 1; i < N; ++i) {
        int pr = dp[0];
        dp[0] = dp[0] || *(f1 + i) == *f2;
        for (size_t j = 1; j < M; ++j) {
            const int prr = dp[j];
            if (*(f1 + i) == *(f2 + j)) dp[j] = pr + 1;
            else dp[j] = max(dp[j], dp[j - 1]);
            pr = prr;
        }
    }
    return dp.back();
}