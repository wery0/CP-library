//O(nlog(n))
template<typename T>
int64_t calc_inversions(const vector<T>& m) {
    const size_t n = m.size();
    vector<T> sm = m;
    sort(sm.begin(), sm.end());
    vector<size_t> fen(n + 1);
    int64_t ans = (int64_t)n * (n - 1) / 2;
    for (const T& elem : m) {
        size_t c = lower_bound(sm.begin(), sm.end(), elem) - sm.begin();
        for (size_t p = c + 1; p; p -= p & -p) ans -= fen[p];
        for (size_t p = c + 1; p <= n; p += p & -p) ++fen[p];
    }
    return ans;
}