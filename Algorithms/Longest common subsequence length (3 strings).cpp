//<O(NMK), O(NMK/max(N, M, K))>
template<typename I>
int longest_common_subsequence_length_3(I f1, I l1, I f2, I l2, I f3, I l3) {
    if (l2 - f2 > l1 - f1) swap(f1, f2), swap(l1, l2);
    if (l3 - f3 > l1 - f1) swap(f1, f3), swap(l1, l3);
    const size_t N = l1 - f1, M = l2 - f2, K = l3 - f3;
    vector<vector<vector<int>>> dp(2, vector<vector<int>>(M + 1, vector<int>(K + 1)));
    for (size_t qq = 1; qq <= N; ++qq) {
        const size_t q = qq & 1;
        for (size_t w = 1; w <= M; ++w) {
            if (*(f1 + qq - 1) == *(f2 + w - 1)) {
                for (size_t e = 1; e <= K; ++e) {
                    if (*(f2 + w - 1) == *(f3 + e - 1)) dp[q][w][e] = dp[q ^ 1][w - 1][e - 1] + 1;
                    else dp[q][w][e] = max({dp[q ^ 1][w][e], dp[q][w - 1][e], dp[q][w][e - 1]});
                }
            } else {
                for (int e = 1; e <= K; ++e) {
                    dp[q][w][e] = max({dp[q ^ 1][w][e], dp[q][w - 1][e], dp[q][w][e - 1]});
                }
            }
        }
    }
    return dp[N & 1][M][K];
}