template<typename T>
int64_t min_adjacent_swaps_to_sort(vector<T>& m) {
    const size_t n = m.size();
    vector<size_t> u(n); iota(u.begin(), u.end(), 0);
    sort(u.begin(), u.end(), [&](const auto& l, const auto& r) {return m[l] < m[r] || (m[l] == m[r] && l < r);});
    vector<int64_t> fen(n + 1);
    int64_t ans = (int64_t)n * (n - 1) / 2;
    for (size_t c : u) {
        for (size_t p = c + 1; p; p -= p & -p) ans -= fen[p];
        for (size_t p = c + 1; p <= n; p += p & -p) ++fen[p];
    }
    return ans;
}