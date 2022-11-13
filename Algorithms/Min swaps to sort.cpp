//WARNING: elements of array must be distinct, otherwise it is NP-Complete!
template<typename T>
int min_swaps_to_sort(vector<T> m) {
    int a = m.size(), cyc_cnt = 0;
    vector<T> n = m;
    vector<int> nm(a);
    vector<bool> us(a);
    sort(all(n));
    for (int q = 1; q < a; ++q) if (n[q] == n[q - 1]) assert(0);
    for (int q = 0; q < a; ++q) nm[q] = lower_bound(all(n), m[q]) - n.begin();
    for (int q = 0; q < a; ++q) {
        if (us[q]) continue;
        for (int w = q; !us[w]; w = nm[w]) us[w] = 1;
        ++cyc_cnt;
    }
    return a - cyc_cnt;
}