//Decreases sum(m) on k, minimizing max remaining element
//Returns sorted array after decreasing
//Constraints: k >= 0, sum of elements must fit into T
//Complexity: O(nlog(n))
template<typename T>
vector<T> remove_k(vector<T> m, T k) {
    if (m.empty() || k < 0) return m;
    const size_t n = m.size();
    sort(m.begin(), m.end());
    for (size_t i = n - 1; ; --i) {
        if (i && m[i] == m[i - 1]) continue;
        T df = i ? m[i] - m[i - 1] : 0, cnt = n - i;
        if (i && df * cnt <= k) {
            k -= df * cnt;
        } else {
            T can = k / cnt, c = m[i];
            fill(m.begin() + i + k % cnt, m.end(), c - can);
            fill(m.begin() + i, m.begin() + i + k % cnt, c - can - 1);
            break;
        }
    }
    return m;
}
