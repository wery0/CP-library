//If no such exists, returns -1.
template<typename I>
vector<int> closest_left_greater(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const int n = last - first;
    vector<int> ans(n, -1);
    stack<pair<T, int>> st;
    for (int q = 0; q < n; ++q, ++first) {
        const auto cur = *first;
        while (!st.empty() && cur >= st.top().first) st.pop();
        if (!st.empty()) ans[q] = st.top().second;
        st.emplace(cur, q);
    }
    return ans;
}

//If no such exists, returns n.
template<typename I>
vector<int> closest_right_greater(I first, I last) {
    using T = typename iterator_traits<I>::value_type;
    const int n = last - first;
    vector<int> ans(n, n);
    stack<pair<T, int>> st;
    for (int q = n - 1; q >= 0; --q) {
        const auto cur = *(--last);
        while (!st.empty() && cur >= st.top().first) st.pop();
        if (!st.empty()) ans[q] = st.top().second;
        st.emplace(cur, q);
    }
    return ans;
}