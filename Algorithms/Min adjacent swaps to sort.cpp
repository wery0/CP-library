template<typename T>
ll min_adjacent_swaps_to_sort(vector<T>& m) {
    int n = m.size();
    vector<int> u(n); iota(all(u), 0);
    sort(all(u), [&](const auto& l, const auto& r) {return m[l] < m[r] || (m[l] == m[r] && l < r);});
    vector<int> fen(n + 1);
    ll ans = 1ll * n * (n - 1) / 2;
    for (int c : u) {
        for (int p = c + 1; p; p -= p & -p) ans -= fen[p];
        for (int p = c + 1; p <= n; p += p & -p) ++fen[p];
    }
    return ans;
}