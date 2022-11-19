template<typename T>
ll kth_lowest_subsequence(vec<T> m, ll k) {
    ll o = 0;
    for (auto &i : m) if (i < 0) o += i, i *= -1;
    if (k-- == 0) return o;
    sort(all(m));
    priority_queue<pair<ll, int>, vec<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({m[0], 0});
    for (; pq.size();) {
        auto [sm, i] = pq.top(); pq.pop();
        if (k-- == 0) return o + sm;
        if (i + 1 != m.size()) {
            pq.push({sm + m[i + 1] - m[i], i + 1});
            pq.push({sm + m[i + 1], i + 1});
        }
    }
    assert(0);
    return -1;
}