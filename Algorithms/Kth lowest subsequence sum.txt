template<typename T>
ll kth_lowest_subsequence_sum(vec<T> m, ll k) {
    ll o = 0;
    for (auto &i : m) if (i < 0) o += i, i *= -1;
    if (k-- == 0) return o;
    sort(all(m));
    priority_queue<pair<ll, int>, vec<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({m[0], 0});
    for (; pq.size();) {
        auto p = pq.top(); pq.pop();
        if (k-- == 0) return o + p.F;
        if (p.S + 1 != m.size()) {
            pq.push({p.F + m[p.S + 1] - m[p.S], p.S + 1});
            pq.push({p.F + m[p.S + 1], p.S + 1});
        }
    }
    assert(0);
    return -1;
}