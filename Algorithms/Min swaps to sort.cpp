//WARNING: elements of array must be distinct, otherwise it is NP-Complete!
template<typename T>
size_t min_swaps_to_sort(vector<T> m) {
    const size_t n = m.size();
    size_t cyc_cnt = 0;
    vector<T> u = m;
    vector<size_t> nm(n);
    vector<bool> us(n);
    sort(u.begin(), u.end());
    for (size_t i = 1; i < n; ++i) if (u[i] == u[i - 1]) assert(0);
    for (size_t i = 0; i < n; ++i) nm[i] = lower_bound(u.begin(), u.end(), m[i]) - u.begin();
    for (size_t i = 0; i < n; ++i) {
        if (us[i]) continue;
        for (size_t w = i; !us[w]; w = nm[w]) us[w] = 1;
        ++cyc_cnt;
    }
    return n - cyc_cnt;
}