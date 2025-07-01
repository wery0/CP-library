//Calculates maximum length of union of k **contiguous** segments out of n given
//<O(knlog(n)) with a good constant, O(n)>
template<typename T>
T max_union_of_k_contiguous_segments(vector<pair<T, T>> m, size_t k) {
    static constexpr T inf = numeric_limits<T>::max();
    const size_t n = m.size();
    k = min(k, n);
    sort(m.begin(), m.end(), [&](const auto& l, const auto& r) {return l.second < r.second;});
    vector<int> bs(n, -1);
    for (int i = 0; i < n; ++i) bs[i] = lower_bound(m.begin(), m.end(), m[i].F, [](const auto& p, T x) {return p.S < x;}) - m.begin() - 1;
    vector dp(2, vector<T>(n));
    for (int i = 0; i < n; ++i) dp[1][i] = max((i ? dp[1][i - 1] : 0), m[i].second - m[i].first);
    vector<pair<int, T>> qq;
    qq.reserve(n);
    auto query = [&](int L) {
        auto it = lower_bound(qq.begin(), qq.end(), L, [&](const auto& p, int x) {return p.F < x;});
        return it == qq.end() ? -inf : (*it).second;
    };
    for (int i = 2; i <= k; ++i) {
        const int I = i & 1;
        fill(dp[I].begin(), dp[I].end(), 0);
        auto add = [&](int p) {
            T val = dp[I ^ 1][p] - m[p].second;
            while (qq.size() && val >= qq.back().second) qq.pop_back();
            qq.emplace_back(p, val);
        };
        qq.clear();
        for (int j = 0; j < i - 2; ++j) add(j);
        for (int j = i - 1; j < n; ++j) {
            add(j - 1);
            dp[I][j] = max({dp[I ^ 1][j], dp[I][j - 1], query(bs[j] + 1) + m[j].second});
            if (bs[j] != -1) dp[I][j] = max(dp[I][j], dp[I ^ 1][bs[j]] + m[j].second - m[j].first);
        }
    }
    return dp[k & 1][n - 1];
}
