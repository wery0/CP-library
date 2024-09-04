//Returns {pos1, pos2, len} which means that s1[pos1 : pos1 + len] == s2[pos2 : pos2 + len] is the longest common substring
//O(nlogn)
template<typename T_arr1, typename T_arr2>
array<size_t, 3> find_longest_common_substring(const T_arr1& s1, const T_arr2& s2) {
    vector<int> s(s1.begin(), s1.end());
    s.insert(s.end(), s2.begin(), s2.end());
    auto mn = *min_element(s.begin(), s.end());
    s.insert(s.begin() + s1.size(), mn - 1);
    s.push_back(mn - 2);
    size_t n = s.size();
    vector<int> sa(n - 1), cl(n), lcp(n - 2), cnt(n), pcl(n), df(s.begin(), s.end());
    sort(df.begin(), df.end());
    df.erase(unique(df.begin(), df.end()), df.end());
    for (size_t i = 0; i < n; ++i) cl[i] = lower_bound(df.begin(), df.end(), s[i]) - df.begin(), ++cnt[cl[i]];
    vector<array<int, 3>> ncl(n);
    for (size_t d = 1; d < n; d *= 2) {
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (size_t i = 0; i < n; ++i) pcl[--cnt[cl[(i + d) % n]]] = i;
        for (int i : pcl) ncl[cnt[cl[i]]++] = {cl[i], cl[(i + d) % n], i};
        for (size_t i = 0; i < n; ++i) cl[ncl[i][2]] = i ? cl[ncl[i - 1][2]] + (ncl[i - 1][0] != ncl[i][0] || ncl[i - 1][1] != ncl[i][1]) : 0;
        memset(&cnt[0], 0, sizeof(cnt[0]) * n);
        for (int i : cl) ++cnt[i];
    }
    s.pop_back();
    for (size_t i = 0; i < n; ++i) if (cl[i]) sa[cl[i] - 1] = i;
    for (size_t i = 0, d = 0; i < s.size(); ++i) {
        int p = cl[i] - 1;
        if (!p) continue;
        size_t j = sa[p - 1];
        d -= !!d;
        while (max(i, j) + d < s.size() && s[i + d] == s[j + d]) ++d;
        lcp[p - 1] = d;
    }
    size_t ps1 = 0, ps2 = s1.size() + 1, mxd = 0;
    for (int q = 1; q + 1 < s.size(); ++q) {
        if ((sa[q] < s1.size()) != (sa[q + 1] < s1.size()) && lcp[q] > mxd) {
            mxd = lcp[q], ps1 = sa[q], ps2 = sa[q + 1];
        }
    }
    if (ps1 > ps2) swap(ps1, ps2);
    ps2 -= s1.size() + 1;
    return {ps1, ps2, mxd};
}
