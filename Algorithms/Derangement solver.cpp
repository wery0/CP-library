//Reorders elements of m such that ans[i] != m[i] for all i, if it's possible.
//O(n)
template<typename T>
vector<T> derangement_solver(const vector<T>& m) {
    const size_t n = m.size();
    umap<T, int> mp;
    vector<int> cnt;
    for (const T& val : m) {
        if (!mp.count(val)) mp[val] = cnt.size(), cnt.push_back(1);
        else ++cnt[mp[val]];
    }
    if (*max_element(cnt.begin(), cnt.end()) * 2 > n) return {};
    partial_sum(cnt.begin(), cnt.end(), cnt.begin());
    vector<int> num(n), gde(n);
    vector<T> ans(n);
    for (size_t i = 0; i < n; ++i) {
        num[i] = --cnt[mp[m[i]]];
        gde[num[i]] = i;
    }
    for (size_t i = 0; i < n; ++i) ans[i] = m[gde[(num[i] + n / 2) % n]];
    return ans;
}
