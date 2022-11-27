//O(n)
template<typename T_arr, typename T>
ll calc_subsegs_with_median_leq(const T_arr& arr, T k) {
    int n = arr.size();
    vector<int> m(n), cnt(n * 2 + 1);
    for (int q = 0; q < n; ++q) {
        m[q] = n + (arr[q] <= k ? -1 : 1);
        m[q] += q ? m[q - 1] - n : 0;
        ++cnt[m[q]];
    }
    ll ans = 0;
    int c = n, leqc = accumulate(cnt.begin(), cnt.begin() + n + 1, 0);
    for (int q = 0; q < m.size(); ++q) {
        ans += leqc;
        --cnt[m[q]];
        if (m[q] <= c) --leqc;
        leqc += m[q] == c + 1 ? cnt[c + 1] : -cnt[c];
        c = m[q];
    }
    return ans;
}