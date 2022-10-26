//if no such exist, returns -1
template<typename I>
vec<int> closest_left_less(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    int a = last - first;
    vec<int> ans(a, -1);
    stack<pair<T, int>> st;
    for (int q = 0; q < a; ++q, ++first) {
        const auto cur = *first;
        while (st.size() && cur <= st.top().F) st.pop();
        if (!st.empty()) ans[q] = st.top().S;
        st.push({cur, q});
    }
    return ans;
}

//if no such exist, returns arr.size()
template<typename I>
vec<int> closest_right_less(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    int a = last - first;
    vec<int> ans(a, a);
    stack<pair<T, int>> st;
    for (int q = a - 1; q >= 0; --q) {
        const auto cur = *(--last);
        while (st.size() && cur <= st.top().F) st.pop();
        if (!st.empty()) ans[q] = st.top().S;
        st.push({cur, q});
    }
    return ans;
}