//For each element returns position of kth greater from right.
//kth_greater_from_right({10, 20, 30, 30, 20, 10, 60, 90}, 2) -> {2, 3, 7, 7, 7, 7, 8, 8}
template<typename T>
vector<int> kth_greater_from_right(const vector<T>& m, int k) {
    int a = m.size();
    vector<int> n(a), ans(a); iota(all(n), 0);
    sort(all(n), [&](const auto & l, const auto & r) {return m[l] > m[r];});
    vector<int> fen(a + 1);
    auto max_pref_with_sum_leq_k = [&](T k) -> int {
        int p = 0, step = 1 << __lg(a + 1);
        T s = 0;
        for (; step; step >>= 1) {
            int nw = p + step;
            if (nw <= a && s + fen[nw] <= k) p = nw, s += fen[nw];
        }
        return p - 1;
    };
    for (int q = 0, r = 0; q < a; ++r, q = r) {
        for (; r + 1 < a && m[n[r + 1]] == m[n[q]];) ++r;
        for (int i = q; i <= r; ++i) {
            int sm = 0;
            for (int p = n[i] + 1; p; p -= p & -p) sm += fen[p];
            ans[n[i]] = max_pref_with_sum_leq_k(sm + k - 1) + 1;
        }
        for (int i = q; i <= r; ++i) {
            for (int p = n[i] + 1; p <= a; p += p & -p) ++fen[p];
        }
    }
    return ans;
}