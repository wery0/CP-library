//Reorders elements of m such that ans[i] != m[i] for all i, if it's possible.
//O(n)
template<typename T>
vector<T> derangement_solver(const vector<T>& m) {
    int n = m.size(), diff = 0;
    umap<T, int> mp;
    vector<int> cnt;
    for (int q = 0; q < n; q++) {
        if (!mp.count(m[q])) mp[m[q]] = diff++, cnt.push_back(1);
        else ++cnt[mp[m[q]]];
    }
    if (*max_element(cnt.begin(), cnt.end()) * 2 > n) return {};
    partial_sum(cnt.begin(), cnt.end(), cnt.begin());
    vector<int> num(n), gde(n);
    vector<T> ans(n);
    for (int q = 0; q < n; q++) {
        num[q] = --cnt[mp[m[q]]];
        gde[num[q]] = q;
    }
    for (int q = 0; q < n; q++) ans[q] = m[gde[(num[q] + n / 2) % n]];
    return ans;
}
