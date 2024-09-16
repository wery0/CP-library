//<O(NMK), O(NMK/max(N, M, K))>
template<typename I>
int longest_common_subsequence_length_3(I f1, I l1, I f2, I l2, I f3, I l3) {
    if (l2 - f2 > l1 - f1) swap(f1, f2), swap(l1, l2);
    if (l3 - f3 > l1 - f1) swap(f1, f3), swap(l1, l3);
    const size_t N = l1 - f1, M = l2 - f2, K = l3 - f3;
    vector dp(2, vector(M + 1, vector<int>(K + 1)));
    for (size_t ii = 1; ii <= N; ++ii) {
        const size_t i = ii & 1;
        for (size_t j = 1; j <= M; ++j) {
            if (*(f1 + ii - 1) == *(f2 + j - 1)) {
                for (size_t k = 1; k <= K; ++k) {
                    if (*(f2 + j - 1) == *(f3 + k - 1)) dp[i][j][k] = dp[i ^ 1][j - 1][k - 1] + 1;
                    else dp[i][j][k] = max({dp[i ^ 1][j][k], dp[i][j - 1][k], dp[i][j][k - 1]});
                }
            } else {
                for (size_t k = 1; k <= K; ++k) { dp[i][j][k] = max({dp[i ^ 1][j][k], dp[i][j - 1][k], dp[i][j][k - 1]}); }
            }
        }
    }
    return dp[N & 1][M][K];
}
