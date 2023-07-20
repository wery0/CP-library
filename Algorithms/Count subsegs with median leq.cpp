//If the length of sequence is even, then its median is the left of the two elements in the middle
//O(n)
template<typename T_arr, typename T>
int64_t calc_subsegs_with_median_leq(const T_arr& arr, T k) {
    const size_t n = arr.size();
    vector<int> m(n), cnt(n * 2 + 1);
    for (size_t i = 0; i < n; ++i) {
        m[i] = n + (arr[i] <= k ? -1 : 1);
        m[i] += i ? m[i - 1] - n : 0;
        ++cnt[m[i]];
    }
    int64_t ans = 0;
    int64_t c = n, leqc = accumulate(cnt.begin(), cnt.begin() + n + 1, 0);
    for (const auto x : m) {
        ans += leqc;
        --cnt[x];
        if (x <= c) --leqc;
        leqc += x == c + 1 ? cnt[c + 1] : -cnt[c];
        c = x;
    }
    return ans;
}
