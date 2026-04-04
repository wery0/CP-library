//Bostan-Mori algorithm for finding kth coefficient of P(X) / Q(X)
//Complexity: O(O(sum_convolution) * log(k))
template<typename T>
T bostan_mori(vector<T> p, vector<T> q, uint64_t k) {
    static_assert(!is_arithmetic_v<T> && "Use custom modulo types!");
    for (; k > 0 && p.size(); k >>= 1) {
        vector<T> mq = q;
        for (size_t i = 1; i < mq.size(); i += 2) mq[i] = -mq[i];
        vector<T> m1 = sum_convolution(p, mq), m2 = sum_convolution(q, mq);
        p.resize(m1.size() / 2), q.resize((m2.size() + 1) / 2);
        for (size_t i = k & 1; i < m1.size(); i += 2) p[i / 2] = m1[i];
        for (size_t i = 0; i < m2.size(); i += 2) q[i / 2] = m2[i];
    }
    return p.size() && q.size() ? p[0] / q[0] : 0;
}
