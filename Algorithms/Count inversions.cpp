//O(nlog(n))
template<typename T>
ll calc_inversions(vector<T> &m) {
    const int n = m.size();
    vector<T> sm = m; sort(all(sm));
    vector<int> u(n);
    for (int q = 0; q < n; ++q) {
        u[q] = lower_bound(all(sm), m[q]) - sm.begin();
    }
    vector<T> fen(n + 1);
    ll ans = 0;
    for (int q = 0; q < n; ++q) {
        ans += q;
        for (int p = u[q] + 1; p; p -= p & -p) ans -= fen[p];
        for (int p = u[q] + 1; p <= n; p += p & -p) ++fen[p];
    }
    return ans;
}