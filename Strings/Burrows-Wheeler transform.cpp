//Burrows-Wheeler (BWT) transform
//Complexity: <O(nlog(n)), O(n)>
template<typename T_arr>
pair<T_arr, int> bwt_encode(T_arr data) {
    using T = T_arr::value_type;
    if (data.empty()) return {{}, -1};
    const int n = data.size();
    vector<int> sa(n), cnt(n), cl(n), pcl(n);
    vector<T> df(data.begin(), data.end());
    sort(df.begin(), df.end()); df.erase(unique(df.begin(), df.end()), df.end());
    for (int i = 0; i < n; ++i) cl[i] = lower_bound(df.begin(), df.end(), data[i]) - df.begin(), ++cnt[cl[i]];
    vector<array<int, 3>> ncl(n);
    for (int d = 1; d < n; d *= 2) {
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (int i = 0; i < n; ++i) pcl[--cnt[cl[(i + d) % n]]] = i;
        for (int i : pcl) ncl[cnt[cl[i]]++] = {cl[i], cl[(i + d) % n], i};
        for (int i = 0; i < n; ++i) cl[ncl[i][2]] = i ? cl[ncl[i - 1][2]] + (ncl[i - 1][0] != ncl[i][0] || ncl[i - 1][1] != ncl[i][1]) : 0;
        memset(&cnt[0], 0, sizeof(cnt[0]) * n);
        for (int i : cl) ++cnt[i];
    }
    partial_sum(cnt.begin(), cnt.end(), cnt.begin());
    for (int i = n - 1; i >= 0; --i) cl[i] = --cnt[cl[i]];
    for (int i = 0; i < n; ++i) sa[cl[i]] = i;
    T_arr encoded(data.size(), T());
    for (int i = 0; i < data.size(); ++i) encoded[i] = data[sa[i] ? sa[i] - 1 : n - 1];
    return {encoded, find(sa.begin(), sa.end(), 0) - sa.begin()};
}

//Burrows-Wheeler (BWT) inverse transform
//Complexity: <O(nlog(distinct characters)), O(n)>
template<typename T_arr>
T_arr bwt_decode(T_arr encoded, int pos) {
    using T = T_arr::value_type;
    if (encoded.empty()) return {};
    const int n = encoded.size();
    map<T, int> cnt;
    for (const auto& i : encoded) ++cnt[i];
    for (int pr = 0, sm = 0; auto& [k, v] : cnt) sm += v, pr = v, v = sm - pr;
    vector<int> t(n);
    for (int i = 0; i < n; ++i) t[i] = cnt[encoded[i]]++;
    T_arr decoded(n, T());
    for (int i = 0; i < n; ++i) {
        decoded[n - 1 - i] = encoded[pos];
        pos = t[pos];
    }
    return decoded;
}
