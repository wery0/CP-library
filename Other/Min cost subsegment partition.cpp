//Divides the array into exactly k subsegments, minimizing the cost
//Returns {mincost, borders of subsegments}, if partition exist. If doesn't, returns {0, {}}
//O(kn^2) cost function calls
//If opt is monotonous in one dimension (opt[i][j] <= opt[i][j + 1]), use D&C to get O(knlogn)
//If opt is monotonous in two dimensions (opt[i - 1][j] <= opt[i][j] <= opt[i][j + 1]), use Knuth to get O(kn)
template<typename T_arr, typename C = int64_t>
pair<C, vector<array<int, 2>>> mincost_subsegment_partition(const T_arr& arr, size_t k) {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    const size_t n = arr.size();
    assert(0 < k && k <= n);
    auto cost = [&](int l, int r) -> C {
        //write
    };
    vector dp(k, vector<C>(n, inf));
    vector opt(k, vector<int>(n, -1));
    for (size_t r = 0; r < n; ++r) dp[0][r] = cost(0, r), opt[0][r] = 0;
    for (size_t i = 1; i < k; ++i) {
        for (size_t r = n - 1; r >= i; --r) {
            for (size_t l = i; l <= r; ++l) {
                C tyt = dp[i - 1][l - 1] + cost(l, r);
                if (tyt < dp[i][r]) {
                    dp[i][r] = tyt;
                    opt[i][r] = l;
                }
            }
        }
    }
    if (opt[k - 1][n - 1] == -1) return {0, {}};
    vector<array<int, 2>> borders;
    for (int i = k - 1, r = n - 1; r != -1; r = opt[i][r] - 1, --i) {
        borders.push_back({opt[i][r], r});
    }
    reverse(borders.begin(), borders.end());
    return {dp[k - 1][n - 1], borders};
}
