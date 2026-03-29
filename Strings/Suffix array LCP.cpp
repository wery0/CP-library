/*
Kasai's arimura's arikawa's li's and pac's algorithm for calculating LCP array for suffix array
Complexity: O(n)
Example:
  s   = "ababba"
  sa  = [5 0 2 4 1 3]
  lcp = [1 2 0 2 1] => lcp(sa[i], sa[i + 1]) = lcp[i]
*/
template<typename T_arr>
vector<int> calc_lcp(const T_arr& s, const vector<int>& sa, bool is_cyclic) {
    const size_t n = sa.size();
    vector<int> pos_in_sa(n), lcp(n - 1);
    for (size_t i = 0; i < n; ++i) pos_in_sa[sa[i]] = i;
    for (size_t i = 0, d = 0; i < n; ++i) {
        int p = pos_in_sa[i];
        if (!p) continue;
        size_t j = sa[p - 1];
        d -= !!d;
        if (is_cyclic) while (d < n && s[(i + d) % n] == s[(j + d) % n]) ++d;
        else while (max(i, j) + d < n && s[i + d] == s[j + d]) ++d;
        lcp[p - 1] = d;
    }
    return lcp;
}
