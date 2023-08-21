//Returns positions of longest increasing subsequence
/*
  Example:
  f({-5, 3, 0, 7}) = {0, 2, 3}
  f({1, 1, 1}) = {2}
*/
template<typename T>
vector<int> LIS_positions(const vector<T>& m) {
	if (m.empty()) return {};
    const size_t n = m.size();
    size_t len = 0, cur_pos = n - 1;
    vector<T> mn(n + 1);
    vector<int> cur_num(n + 1), pr(n + 1);
    cur_num[0] = pr[0] = -1;
    for (size_t i = 0; i < n; ++i) {
        size_t ps = lower_bound(mn.begin() + 1, mn.begin() + len + 1, m[i]) - mn.begin();
        mn[ps] = m[i];
        cur_num[ps] = i;
        pr[i] = cur_num[ps - 1];
        len = max(len, ps);
    }
    while (m[cur_pos] != mn[len]) --cur_pos;
    vector<int> ans(len);
    for (size_t i = 0; i < len; ++i, cur_pos = pr[cur_pos]) ans[len - 1 - i] = cur_pos;
    return ans;
}
