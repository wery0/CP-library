template<typename I>
vec<int> Z(I first, I last) {
    int a = last - first;
    vec<int> m(a);
    for (int q = 1, l = 0, r = 0; q < a; ++q) {
        m[q] = max(0, min(r - q, m[q - l]));
        while (q + m[q] < a && *(first + m[q]) == *(first + m[q] + q)) {
            ++m[q];
        }
        l = chmax(r, q + m[q]) ? q : l;
    }
    return m;
}

//finds all left shifts of m, which equals to n.
template<typename T_arr>
vec<int> find_all_left_shifts(T_arr m, T_arr n) {
    int a = isz(m); assert(isz(n) == a);
    n.pb(-1);            //must not appear in m or n
    n.insert(n.end(), all(m));
    n.insert(n.end(), all(m));
    vec<int> zf = Z(all(n));
    vec<int> ans;
    for (int q = a + 1, i = 0; i < a; ++q, ++i) {
        if (zf[q] == a) ans.pb(i);
    }
    return ans;
}