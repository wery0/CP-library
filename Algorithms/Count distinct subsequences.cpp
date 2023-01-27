//Including empty subsequence.
template<typename T, typename I>
T count_distinct_subsequences(I first, I last) {
    const int n = last - first;
    vector<T> dp(n + 1);
    umap<T, int> mp;
    dp[0] = 1;
    auto cit = first;
    for (int q = 1; q <= n; ++q, ++cit) {
        const auto x = *cit;
        dp[q] = dp[q - 1] * 2 - (mp.count(x) ? dp[mp[x] - 1] : 0);
        mp[x] = q;
    }
    return dp[n];
}