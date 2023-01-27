//For each element returns position of kth greater from right.
//kth_greater_from_right({10, 20, 30, 30, 20, 10, 60, 90}, 2) -> {2, 3, 7, 7, 7, 7, 8, 8}
template<typename T>
vector<int> kth_greater_from_right(const vector<T>& m, int k) {
    const int n = m.size();
    vector<int> u(n), ans(n); 
    iota(u.begin(), u.end(), 0);
    sort(u.begin(), u.end(), [&](const auto& l, const auto& r) {return m[l] > m[r];});
    vector<int> fen(n + 1);
    auto max_pref_with_sum_leq_k = [&](T k) -> int {
        int p = 0, step = 1 << __lg(n + 1);
        T s = 0;
        for (; step; step >>= 1) {
            int nw = p + step;
            if (nw <= n && s + fen[nw] <= k) p = nw, s += fen[nw];
        }
        return p - 1;
    };
    for (int q = 0, r = 0; q < n; ++r, q = r) {
        for (; r + 1 < n && m[u[r + 1]] == m[u[q]];) ++r;
        for (int i = q; i <= r; ++i) {
            int sm = 0;
            for (int p = u[i] + 1; p; p -= p & -p) sm += fen[p];
            ans[u[i]] = max_pref_with_sum_leq_k(sm + k - 1) + 1;
        }
        for (int i = q; i <= r; ++i) {
            for (int p = u[i] + 1; p <= n; p += p & -p) ++fen[p];
        }
    }
    return ans;
}