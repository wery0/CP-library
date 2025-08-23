//Returns length of longest common subsequence.
//<O(min(N, M) + klog(min(N, M))), O(min(N, M))>, where k = |{(i, j) | s[i] == t[j]}|
template<typename I>
ssize_t hunt_szymanski_LCS_length(I f1, I l1, I f2, I l2) {
    using T = typename iterator_traits<I>::value_type;
    if (l1 - f1 > l2 - f2) swap(f1, f2), swap(l1, l2);
    const ssize_t N = l1 - f1, M = l2 - f2;
    map<T, vector<size_t>> gde;
    for (ssize_t i = N - 1; i >= 0; --i) gde[*(f1 + i)].push_back(i + 1);
    vector<size_t> dp(N + 2, N + 1);
    dp[0] = 0;
    for (auto i2 = f2; i2 != l2; ++i2) {
        for (size_t pos : gde[*i2]) {
            auto it = lower_bound(dp.begin(), dp.end(), pos);
            *it = min(*it, pos);
        }
    }
    return lower_bound(dp.begin(), dp.end(), N + 1) - dp.begin() - 1;
}

//Returns longest common subsequence itself.
//<O(NlogN + klog(N)), O(N + k)>, where k = |{(i, j) | s[i] == t[j]}|
template<typename I>
auto hunt_szymanski_LCS(I f1, I l1, I f2, I l2) {
    using T = typename iterator_traits<I>::value_type;
    //if (l1 - f1 > l2 - f2) swap(f1, f2), swap(l1, l2);
    const ssize_t N = l1 - f1, M = l2 - f2;
    map<T, vector<size_t>> gde;
    for (ssize_t i = N - 1; i >= 0; --i) gde[*(f1 + i)].push_back(i + 1);
    vector<size_t> dp(N + 2, N + 1);
    vector<pair<size_t, T>> store;
    dp[0] = 0;
    for (auto i2 = f2; i2 != l2; ++i2) {
        for (size_t pos : gde[*i2]) {
            auto it = lower_bound(dp.begin(), dp.end(), pos);
            store.emplace_back(it - dp.begin(), *it);
            *it = min(*it, pos);
        }
    }
    reverse(store.begin(), store.end());
    size_t len = lower_bound(dp.begin(), dp.end(), N + 1) - dp.begin() - 1;
    vector<T> ans(len);
    for (auto [pos, old] : store) {
        if (pos == len) ans[--len] = *(f1 + dp[pos] - 1);
        dp[pos] = old;
    }
    return ans;
}
