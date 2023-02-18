//Returns positions of longest increasing subsequence.
/*
Example:
f({-5, 3, 0, 7}) = {0, 2, 3}
f({1, 1, 1}) = {2}
*/
template<typename T>
vector<int> LIS_positions(vector<T>& m) {
    int n = m.size(), len = 0, cur_pos = n - 1;
    vector<T> mn(n + 1);
    vector<int> cur_num(n + 1), pr(n + 1);
    cur_num[0] = pr[0] = -1;
    for (int q = 0; q < n; ++q) {
        int ps = lower_bound(mn.begin() + 1, mn.begin() + len + 1, m[q]) - mn.begin();
        mn[ps] = m[q];
        cur_num[ps] = q;
        pr[q] = cur_num[ps - 1];
        chmax(len, ps);
    }
    while (m[cur_pos] != mn[len]) --cur_pos;
    vector<int> ans(len);
    for (int i = len - 1; i >= 0; --i, cur_pos = pr[cur_pos]) ans[i] = cur_pos;
    return ans;
}