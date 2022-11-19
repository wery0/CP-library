//Sum of all elements must fit in T
template<typename T>
T kth_lowest_subsequence(vector<T> m, ll k) {
    T o = 0;
    for (T &i : m) if (i < 0) o += i, i *= -1;
    if (k-- == 0) return o;
    sort(all(m));
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
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