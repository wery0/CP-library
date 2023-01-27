//If no such exists, returns -1.
template<typename I>
vector<int> closest_left_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const int n = last - first;
    vector<int> ans(n, -1);
    umap<T, int> mp;
    auto cit = first;
    for (int q = 0; q < n; ++q, ++cit) {
        T cur = *cit;
        if (mp.count(cur)) ans[q] = mp[cur];
        mp[cur] = q;
    }
    return ans;
}

//If no such exists, returns n.
template<typename I>
vector<int> closest_right_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const int n = last - first;
    vector<int> ans(n, n);
    umap<T, int> mp;
    auto cit = last;
    for (int q = n - 1; q >= 0; --q) {
        T cur = *(--last);
        if (mp.count(cur)) ans[q] = mp[cur];
        mp[cur] = q;
    }
    return ans;
}