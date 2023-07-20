//Returns vector dp s. t. dp[x] = sum({m[y] | (x & y) == y})
//O(nlog(n))
template<typename T>
vector<T> sum_over_subsets(const vector<T>& m) {
    const size_t n = m.size(), lg = __lg(n & (n - 1) ? 2 << __lg(n) : n);
    vector<T> dp = m;
    for (size_t bit = 0; bit < lg; ++bit) {
        for (size_t i = 0; i < n; ++i) {
            if (i >> bit & 1) dp[i] += dp[i ^ (1 << bit)];
        }
    }
    return dp;
}