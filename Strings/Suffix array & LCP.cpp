//O(nlog(n))
vector<int> calc_suffix_array(string& t) {
    t += (char)(0);           //This symbol must not appear in the string
    int n = t.size();
    vector<int> cnt(n), cl(n), pcl(n);
    int kek[256] = {0};
    for (char c : t) kek[c] = 1;
    for (int q = 0, i = 0; q < 256; q++) if (kek[q]) kek[q] = i++;
    for (int q = 0; q < n; ++q) cl[q] = kek[t[q]], ++cnt[cl[q]];
    vector<pair<pair<int, int>, int>> ncl(n);
    for (int d = 1; d < n; d *= 2) {
        for (int q = 1; q < n; q++) cnt[q] += cnt[q - 1];
        for (int q = 0; q < n; q++) pcl[--cnt[cl[(q + d) % n]]] = q;
        for (int q : pcl) ncl[cnt[cl[q]]++] = {{cl[q], cl[(q + d) % n]}, q};
        for (int q = 0; q < n; q++) cl[ncl[q].second] = q ? cl[ncl[q - 1].second] + (ncl[q - 1].first != ncl[q].first) : 0;
        fill(all(cnt), 0);
        for (int i : cl) cnt[i]++;
    }
    vector<int> suf(n - 1);
    for (int q = 0; q < n; ++q) if (cl[q]) suf[cl[q] - 1] = q;
    return suf;
}

//O(n)
vector<int> calc_lcp(string& t, vector<int>& suf) {
    int n = suf.size();
    vector<int> pos_in_sufmas(n), lcp(n - 1);
    for (int q = 0; q < n; ++q) pos_in_sufmas[suf[q]] = q;
    for (int q = 0; q < n - 1; ++q) {
        int i = pos_in_sufmas[q];
        if (!i) continue;
        if (q) {
            int prps = pos_in_sufmas[q - 1] - 1;
            lcp[i - 1] = max(0, prps >= 0 ? lcp[prps] - 1 : 0);
        }
        for (int d = lcp[i - 1]; q + d < n && suf[i - 1] + d < n && t[q + d] == t[suf[i - 1] + d]; ++d) ++lcp[i - 1];
    }
    return lcp;
}
// ababba
// suf = [5 0 2 4 1 3]
// lcp = [1 2 0 2 1] => lcp(suf[i], suf[i + 1]) = lcp[i]