//If no such exists, returns -1.
template<typename I>
vector<int> closest_left_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const size_t n = std::distance(first, last);
    vector<int> ans(n, -1);
    umap<T, int> mp;
    for (size_t i = 0; i < n; ++i) {
        T cur = *(first++);
        if (mp.count(cur)) ans[i] = mp[cur];
        mp[cur] = i;
    }
    return ans;
}

//If no such exists, returns n.
template<typename I>
vector<int> closest_right_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const size_t n = std::distance(first, last);
    vector<int> ans(n, n);
    umap<T, int> mp;
    for (size_t i = n; i;) {
        T cur = *(--last);
        if (mp.count(cur)) ans[--i] = mp[cur];
        mp[cur] = i;
    }
    return ans;
}
