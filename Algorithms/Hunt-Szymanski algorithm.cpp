//Returns length of longest common subsequence.
//<O(min(N, M) + klog(min(N, M))), O(min(N, M))>, where k = |{(i, j) | s[i] == t[j]}|
template<typename I>
int hunt_szymanski_LCS_length(I f1, I l1, I f2, I l2) {
    //Change, if need
    static constexpr ALPHABET = 26;
    static constexpr FIRST_CHAR = 'a';

    if (l1 - f1 > l2 - f2) swap(f1, f2), swap(l1, l2);
    const int N = l1 - f1, M = l2 - f2;
    vector<vector<int>> gde(ALPHABET);
    for (int q = N - 1; q >= 0; --q) {
        gde[*(f1 + q) - FIRST_CHAR].push_back(q + 1);
    }
    vector<int> dp(N + 2, N + 1);
    dp[0] = 0;
    for(auto i2 = f2; i2 != l2; ++i2) {
        const char c = *i2;
        for (int pos : gde[c - FIRST_CHAR]) {
            auto it = lower_bound(dp.begin(), dp.end(), pos);
            *it = min(*it, pos);
        }
    }
    return lower_bound(dp.begin(), dp.end(), N + 1) - dp.begin() - 1;
}