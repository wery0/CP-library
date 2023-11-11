//Both array must be sorted
//O(log(k)) comparator calls
template<typename T>
T kth_ordered_statistic_of_two_sorted_arrays(const vector<T>& m1, const vector<T>& m2, int k) {
    auto cmp = [&](int u, int i, int j) {
        if (u) swap(i, j);
        int res = m1[i] < m2[j];
        return res ^ u;
    };
    auto go = [&](auto&& go, int u, int i1, int n1, int i2, int n2, int k) -> array<int, 2> {
        if (n1 > n2) return go(go, u ^ 1, i2, n2, i1, n1, k);
        if (n1 == 0) return {u ^ 1, i2 + k};
        if (k == 0) {
            if (cmp(u, i1, i2)) return {u, i1};
            return {u ^ 1, i2};
        }
        int i = min(n1, (k + 1) / 2);
        int j = min(n2, (k + 1) / 2);
        int fl = cmp(u, i1 + i - 1, i2 + j - 1);
        return fl ? go(go, u, i1 + i, n1 - i, i2, n2, k - i) :
               go(go, u, i1, n1, i2 + j, n2 - j, k - j);
    };
    assert(k < m1.size() + m2.size());
    auto [u, i] = go(go, 0, 0, m1.size(), 0, m2.size(), k);
    return (u == 0 ? m1 : m2)[i];
}
