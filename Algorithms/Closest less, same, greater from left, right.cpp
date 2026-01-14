enum VALUE_TYPE {LESS, SAME, GREATER};
//If no such exists, returns -1.
template<typename I>
vector<int> closest_left(I first, I last, VALUE_TYPE type) {
    using T = typename iterator_traits<I>::value_type;
    const size_t n = std::distance(first, last);
    vector<int> ans(n, -1);
    if (type == LESS) {
        stack<pair<T, int>> st;
        for (int i = 0; i < n; ++i, ++first) {
            const auto cur = *first;
            while (!st.empty() && cur <= st.top().first) st.pop();
            if (!st.empty()) ans[i] = st.top().second;
            st.emplace(cur, i);
        }
    } else if (type == GREATER) {
        stack<pair<T, int>> st;
        for (int i = 0; i < n; ++i, ++first) {
            const auto cur = *first;
            while (!st.empty() && cur >= st.top().first) st.pop();
            if (!st.empty()) ans[i] = st.top().second;
            st.emplace(cur, i);
        }
    } else if (type == SAME) {
        umap<T, int> mp;
        for (size_t i = 0; i < n; ++i) {
            T cur = *(first++);
            if (mp.count(cur)) ans[i] = mp[cur];
            mp[cur] = i;
        }
    } else assert(0);
    return ans;
}

//If no such exists, returns n.
template<typename I>
vector<int> closest_right(I first, I last, VALUE_TYPE type) {
    using T = typename iterator_traits<I>::value_type;
    const size_t n = std::distance(first, last);
    vector<int> ans(n, n);
    if (type == LESS) {
        stack<pair<T, int>> st;
        for (int i = int(n) - 1; i >= 0; --i) {
            const auto cur = *(--last);
            while (!st.empty() && cur <= st.top().first) st.pop();
            if (!st.empty()) ans[i] = st.top().second;
            st.emplace(cur, i);
        }
    } else if (type == GREATER) {
        stack<pair<T, int>> st;
        for (int i = int(n) - 1; i >= 0; --i) {
            const auto cur = *(--last);
            while (!st.empty() && cur >= st.top().first) st.pop();
            if (!st.empty()) ans[i] = st.top().second;
            st.emplace(cur, i);
        }
    } else if (type == SAME) {
        umap<T, int> mp;
        for (int i = int(n) - 1; i >= 0; --i) {
            T cur = *(--last);
            if (mp.count(cur)) ans[i] = mp[cur];
            mp[cur] = i;
        }
    } else assert(0);
    return ans;
}
