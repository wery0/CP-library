vec<int> sufmas(str &t) {
    t += (char)(0);           //This symbol must not appear in the string
    int a = t.size(), cnt[a] = {0}, cl[a], pcl[a], kek[256] = {0};
    for (char c : t) kek[c] = 1;
    for (int q = 0, i = 0; q < 256; q++) if (kek[q]) kek[q] = i++;
    for (int q = 0; q < a; q++) cl[q] = kek[t[q]], cnt[cl[q]]++;
    pair<pii, int> ncl[a];
    for (int d = 1; d < a; d *= 2) {
        for (int q = 1; q < a; q++) cnt[q] += cnt[q - 1];
        for (int q = 0; q < a; q++) pcl[--cnt[cl[(q + d) % a]]] = q;
        for (int q : pcl) ncl[cnt[cl[q]]++] = {{cl[q], cl[(q + d) % a]}, q};
        for (int q = 0; q < a; q++) cl[ncl[q].S] = q ? cl[ncl[q - 1].S] + (ncl[q - 1].F != ncl[q].F) : 0;
        memset(cnt, 0, sizeof(cnt));
        for (int i : cl) cnt[i]++;
    }
    vec<int> suf(a - 1);
    for (int q = 0; q < a; q++) if (cl[q]) suf[cl[q] - 1] = q;
    return suf;
}

vec<int> calc_lcp(str &t, vec<int> &suf) {
    int a = suf.size();
    vec<int> pos_in_sufmas(a), lcp(a - 1);
    for (int q = 0; q < a; q++) pos_in_sufmas[suf[q]] = q;
    for (int q = 0; q < a - 1; q++) {
        int i = pos_in_sufmas[q];
        if (!i) continue;
        if (q) {
            int prps = pos_in_sufmas[q - 1] - 1;
            lcp[i - 1] = max(0, prps >= 0 ? lcp[prps] - 1 : 0);
        }
        for (int d = lcp[i - 1]; q + d < a && suf[i - 1] + d < a && t[q + d] == t[suf[i - 1] + d]; d++) lcp[i - 1]++;
    }
    return lcp;
}
// ababba
// suf = [5 0 2 4 1 3]
// lcp = [1 2 0 2 1] \Rightarrow lcp(suf[i], suf[i + 1]) = lcp[i]