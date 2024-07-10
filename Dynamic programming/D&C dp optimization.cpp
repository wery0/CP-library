//Divides the array into exactly k subsegments, minimizing the cost
//Returns {mincost, borders of subsegments}, if partition exist. If doesn't, returns {0, {}}
//seg_cost must be function in form C(int l, int r, int seg_num)
//<O(knlog(n) * T(cost)), O(nk)>
//Requirement: opt is monotonous in one dimension, i. e. opt[i][j] <= opt[i][j + 1]
template<typename C = int64_t>
pair<C, vector<array<int, 2>>>
mincost_subsegment_partition_with_answer_restore_divide_and_conquer(const size_t n, const size_t k, auto seg_cost) {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    assert(0 < k && k <= n);
    vector dp(k, vector<C>(n, inf));
    vector opt(k, vector<int>(n, -1));
    for (size_t r = 0; r < n; ++r) dp[0][r] = seg_cost(0, r, 0), opt[0][r] = 0;
    for (size_t i = 1; i < k; ++i) {
        auto go = [&](auto&& go, const size_t l, const size_t r, const size_t opt_l, const size_t opt_r) -> void {
            const size_t md = (l + r) / 2;
            size_t opt_md;
            for (size_t t = opt_l; t <= min(opt_r, md); ++t) {
                C tyt = dp[i - 1][t - 1] + seg_cost(t, md, i);
                if (tyt < dp[i][md]) {
                    dp[i][md] = tyt;
                    opt[i][md] = t;
                }
            }
            if (l < md) go(go, l, md - 1, opt_l, opt[i][md]);
            if (r > md) go(go, md + 1, r, opt[i][md], opt_r);
        };
        go(go, i, n - 1, i, n - 1);
    }
    if (dp[k - 1][n - 1] == inf) return {0, {}};
    vector<array<int, 2>> borders(k);
    for (int i = k - 1, r = n - 1; i >= 0; r = opt[i][r] - 1, --i) {
        assert(r != -1);
        borders[i] = {opt[i][r], r};
    }
    return {dp[k - 1][n - 1], borders};
}

//Returns mincost of partition. If it doesn't exist, returns 0.
//<O(knlog(n) * T(cost)), O(n)>
template<typename C = int64_t>
C mincost_subsegment_partition_without_answer_restore_divide_and_conquer(const size_t n, const size_t k, auto seg_cost) {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    assert(0 < k && k <= n);
    vector dp(2, vector<C>(n, inf));
    for (size_t r = 0; r < n; ++r) dp[0][r] = seg_cost(0, r, 0);
    for (size_t i = 1; i < k; ++i) {
        const size_t ii = i & 1;
        fill(dp[ii].begin(), dp[ii].end(), inf);
        auto go = [&](auto&& go, const size_t l, const size_t r, const size_t opt_l, const size_t opt_r) -> void {
            const size_t md = (l + r) / 2;
            size_t opt_md;
            for (size_t t = opt_l; t <= min(opt_r, md); ++t) {
                C tyt = dp[ii ^ 1][t - 1] + seg_cost(t, md, i);
                if (tyt < dp[ii][md]) {
                    dp[ii][md] = tyt;
                    opt_md = t;
                }
            }
            if (l < md) go(go, l, md - 1, opt_l, opt_md);
            if (r > md) go(go, md + 1, r, opt_md, opt_r);
        };
        go(go, i, n - 1, i, n - 1);
    }
    if (dp[~k & 1][n - 1] == inf) return 0;
    return dp[~k & 1][n - 1];
}
