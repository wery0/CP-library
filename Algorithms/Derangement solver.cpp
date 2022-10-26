template<typename T>
vec<T> derangement_solver(vec<T> &m) {
    int a = m.size(), diff = 0;
    umap<T, int> mp;
    vec<int> cnt;
    for (int q = 0; q < a; q++) {
        if (!mp.count(m[q])) mp[m[q]] = diff++, cnt.pb(1);
        else ++cnt[mp[m[q]]];
    }
    if (*max_element(all(cnt)) * 2 > a) return {};
    for (int q = 1; q < diff; q++) cnt[q] += cnt[q - 1];
    vec<int> num(a), gde(a);
    vec<T> ans(a);
    for (int q = 0; q < a; q++) {
        num[q] = --cnt[mp[m[q]]];
        gde[num[q]] = q;
    }
    for (int q = 0; q < a; q++) ans[q] = m[gde[(num[q] + a / 2) % a]];
    return ans;
}

template<typename Iterator>
auto derangement_solver(Iterator first, Iterator last) {
    using T = typename iterator_traits<Iterator>::value_type;
    auto m = vector(first, last);
    int a = m.size(), diff = 0;
    umap<T, int> mp;
    vec<int> cnt;
    for (int q = 0; q < a; q++) {
        if (!mp.count(m[q])) mp[m[q]] = diff++, cnt.pb(1);
        else ++cnt[mp[m[q]]];
    }
    if (*max_element(all(cnt)) * 2 > a) return vec<T>();
    for (int q = 1; q < diff; q++) cnt[q] += cnt[q - 1];
    vec<int> num(a), gde(a);
    vec<T> ans(a);
    for (int q = 0; q < a; q++) {
        num[q] = --cnt[mp[m[q]]];
        gde[num[q]] = q;
    }
    for (int q = 0; q < a; q++) ans[q] = m[gde[(num[q] + a / 2) % a]];
    return ans;
}