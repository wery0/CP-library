//O(nlog(n))
vector<int> calc_suffix_array(string& t) {
    t += (char)(0);           //This symbol must not appear in the string
    const int n = t.size();
    vector<int> cnt(n), cl(n), pcl(n), kek(256);
    for (char c : t) kek[c] = 1;
    for (int i = 0, j = 0; i < 256; ++i) if (kek[i]) kek[i] = j++;
    for (int i = 0; i < n; ++i) cl[i] = kek[t[i]], ++cnt[cl[i]];
    vector<array<int, 3>> ncl(n);
    for (int d = 1; d < n; d *= 2) {
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (int i = 0; i < n; ++i) pcl[--cnt[cl[(i + d) % n]]] = i;
        for (int i : pcl) ncl[cnt[cl[i]]++] = {cl[i], cl[(i + d) % n], i};
        for (int i = 0; i < n; ++i) cl[ncl[i][2]] = i ? cl[ncl[i - 1][2]] + (ncl[i - 1][0] != ncl[i][0] || ncl[i - 1][1] != ncl[i][1]) : 0;
        memset(&cnt[0], 0, sizeof(cnt[0]) * n);
        for (int i : cl) ++cnt[i];
    }
    t.pop_back();
    vector<int> suf(n - 1);
    for (int i = 0; i < n; ++i) if (cl[i]) suf[cl[i] - 1] = i;
    return suf;
}

//O(n)
vector<int> calc_lcp(const string& t, const vector<int>& suf) {
    const int n = suf.size();
    vector<int> pos_in_sufmas(n), lcp(n - 1);
    for (int i = 0; i < n; ++i) pos_in_sufmas[suf[i]] = i;
    for (int i = 0; i < n - 1; ++i) {
        int p = pos_in_sufmas[i];
        if (!p) continue;
        if (i) {
            int prps = pos_in_sufmas[i - 1] - 1;
            lcp[p - 1] = max(0, prps >= 0 ? lcp[prps] - 1 : 0);
        }
        for (int d = lcp[p - 1]; i + d < n && suf[p - 1] + d < n && t[i + d] == t[suf[p - 1] + d]; ++d) ++lcp[p - 1];
    }
    return lcp;
}
// ababba
// suf = [5 0 2 4 1 3]
// lcp = [1 2 0 2 1] => lcp(suf[i], suf[i + 1]) = lcp[i]