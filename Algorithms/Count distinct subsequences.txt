template<typename T>
ll count_distinct_subseq(vec<T> &m, const ll mod) {  //including empty
    int a = m.size();
    vec<ll> dp(a + 1);
    umap<T, int> mp;
    dp[0] = 1;
    for (int q = 1; q <= a; ++q) {
        const T x = m[q - 1];
        dp[q] = dp[q - 1] * 2 + mod - (mp.count(x) ? dp[mp[x] - 1] : 0);
        if (dp[q] > mod) dp[q] -= mod;
        if (dp[q] > mod) dp[q] -= mod;
        mp[x] = q;
    }
    return dp[a];
}