template<typename I>
vec<int> closest_left_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    int a = last - first;
    vec<int> ans(a, -1);
    umap<T, int> mp;
    auto cit = first;
    for (int q = 0; q < a; ++q, ++cit) {
        T cur = *cit;
        if (mp.count(cur)) {
            ans[q] = mp[cur];
        }
        mp[cur] = q;
    }
    return ans;
}

template<typename I>
vec<int> closest_right_eq(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    int a = last - first;
    vec<int> ans(a, a);
    umap<T, int> mp;
    auto cit = last;
    for (int q = a - 1; q >= 0; --q) {
        T cur = *(--last);
        if (mp.count(cur)) {
            ans[q] = mp[cur];
        }
        mp[cur] = q;
    }
    return ans;
}