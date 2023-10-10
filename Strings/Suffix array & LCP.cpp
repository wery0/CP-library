//O(nlog(n))
template<typename T_arr>
vector<int> calc_suffix_array(T_arr& s) {
    using T = T_arr::value_type;
    s.push_back(0);           //This symbol must not appear in the sequence and must be the smallest one
    const size_t n = s.size();
    vector<int> cnt(n), cl(n), pcl(n);
    vector<T> df(s.begin(), s.end());
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
    vector<int> sa(n - 1);
    for (size_t i = 0; i < n; ++i) if (cl[i]) sa[cl[i] - 1] = i;
    return sa;
}

/*
s   = "ababba"
sa  = [5 0 2 4 1 3]
lcp = [1 2 0 2 1] => lcp(sa[i], sa[i + 1]) = lcp[i]
*/
//Kasai's arimura's arikawa's li's and pac's algorithm
//O(n)
template<typename T_arr>
vector<int> calc_lcp(const T_arr& s, const vector<int>& sa) {
    const size_t n = sa.size();
    vector<int> pos_in_sa(n), lcp(n - 1);
    for (size_t i = 0; i < n; ++i) pos_in_sa[sa[i]] = i;
    for (size_t i = 0, d = 0; i < n; ++i) {
        int p = pos_in_sa[i];
        if (!p) continue;
        size_t j = sa[p - 1];
        d -= !!d;
        while (max(i, j) + d < n && s[i + d] == s[j + d]) ++d;
        lcp[p - 1] = d;
    }
    return lcp;
}
