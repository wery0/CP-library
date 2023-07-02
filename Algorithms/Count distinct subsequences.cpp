//Including empty subsequence
//O(n)
template<typename T, typename I>
T count_distinct_subsequences(I first, I last) {
    const size_t n = last - first;
    vector<T> dp(n + 1);
    unordered_map<typename iterator_traits<I>::value_type, size_t> mp;
    dp[0] = 1;
    auto cit = first;
    for (size_t i = 1; i <= n; ++i, ++cit) {
        const auto x = *cit;
        dp[i] = dp[i - 1] * (T)2 - (mp.count(x) ? dp[mp[x] - 1] : (T)0);
        mp[x] = i;
    }
    return dp[n];
}