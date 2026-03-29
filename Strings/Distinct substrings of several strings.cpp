//Calculates number of distinct substrings across all given strings
//Complexity: the same as calculating suffix and lcp arrays for a sequence of length n
template<typename T_arr>
uint64_t calc_distinct_substrings_of_several_strings(vector<T_arr> m) {
    using T = T_arr::value_type;
    if (m.empty()) return 0;
    int n = m.size();
    for (const auto& s : m) n += s.size(); 
    T_arr t; t.reserve(n);
    const T inf = numeric_limits<T>::max();
    for (const auto& s : m) {
        assert(all_of(s.begin(), s.end(), [&](const auto& i) {return i < inf;}));
        t.insert(t.end(), s.begin(), s.end());
        t.push_back(inf);
    }
    vector<int> nxt(n, n);
    for (int i = n - 2; i >= 0; --i) nxt[i] = t[i + 1] == inf ? i + 1 : nxt[i + 1];
    auto sa = calc_suffix_array(t, false);
    auto lcp = calc_lcp(t, sa, false);
    uint64_t o = 0;
    for (int i = 0; i < t.size(); ++i) {
        if (t[sa[i]] == inf) continue;
        int d = nxt[sa[i]] - sa[i];
        int lc = i ? lcp[i - 1] : 0;
        o += max(d - lc, 0);
    }
    return o;
}
