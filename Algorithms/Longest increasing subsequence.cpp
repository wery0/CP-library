template<typename T>
vector<int> LIS_positions(vector<T> &m) {
    int a = m.size(), len = 0, cur_pos = a - 1;
    vector<T> mn(a + 1);
    vector<int> cur_num(a + 1), pr(a + 1);
    cur_num[0] = pr[0] = -1;
    for (int q = 0; q < a; ++q) {
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