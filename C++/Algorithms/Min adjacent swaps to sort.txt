template<typename T>
ll min_adjacent_swaps_to_sort(vec<T> &m) {
    int a = m.size();
    vec<int> n(a); iota(all(n), 0);
    sort(all(n), [&](const auto & l, const auto & r) {return m[l] < m[r] || (m[l] == m[r] && l < r);});
    vec<int> fen(a + 1);
    ll o = 1ll * a * (a - 1) / 2;
    for (int c : n) {
        for (int p = c + 1; p; p -= p & -p) o -= fen[p];
        for (int p = c + 1; p <= a; p += p & -p) ++fen[p];
    }
    return o;
}