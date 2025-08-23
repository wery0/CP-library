//Sum of all elements must fit in T
//O(nlogn + klogk)
template<typename T>
T kth_lowest_subsequence_sum(vector<T> m, size_t k) {
    T ans = 0;
    for (T& i : m) if (i < 0) ans += i, i *= -1;
    if (k-- == 0) return ans;
    sort(m.begin(), m.end());
    priority_queue<pair<T, size_t>, vector<pair<T, size_t>>, greater<pair<T, size_t>>> pq;
    pq.emplace(m[0], 0);
    while (!pq.empty()) {
        auto [sm, i] = pq.top(); pq.pop();
        if (k-- == 0) return ans + sm;
        if (i + 1 != m.size()) {
            pq.emplace(sm + m[i + 1] - m[i], i + 1);
            pq.emplace(sm + m[i + 1], i + 1);
        }
    }
    assert(0 && "k must be < 2^n");
    return -1;
}
