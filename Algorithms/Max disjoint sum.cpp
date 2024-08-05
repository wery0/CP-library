//https://cses.fi/problemset/task/2426/
//https://codeforces.com/contest/1799/problem/F
template<typename T>
class max_disjoint_sum {
    vector<array<T, 2>> m;

public:
    max_disjoint_sum() = default;

    template<typename I1, typename I2>
    max_disjoint_sum(I1 first1, I1 last1, I2 first2, I2 last2) {
        const size_t n = std::distance(first1, last1);
        assert(n == std::distance(first2, last2));
        m.resize(n);
        for (size_t i = 0; i < n; ++i, ++first1, ++first2) {
            m[i] = {*first1, *first2};
        }
    }

    void add_pair(T x, T y) {m.push_back({x, y});}
    void clear() {m.clear();}
    void reserve(size_t capacity) {m.reserve(capacity);}

    //Returns maximum sum that you can achieve by selecting k0 first elements of pairs and k1 second elements of other pairs.
    //O(nlog(n))
    T calc_max_disjoint_sum(size_t k0, size_t k1) {
        const size_t n = m.size();
        assert(k0 + k1 <= n);
        nth_element(m.rbegin(), m.rend() - k0, m.rend());
        nth_element(m.rbegin(), m.rend() - k0 - k1, m.rend() - k0);
        sort(m.rend() - k0 - k1, m.rend() - k0);
        priority_queue<T, vector<T>, greater<T>> pqr1;
        priority_queue<T, vector<T>, greater<T>> pqkillr1;
        priority_queue<T, vector<T>, less<T>> pqdf;
        T sr0 = 0, sdf = 0, sr1 = 0;
        for (size_t i = 0; i < k0; ++i) {
            sr0 += m[i][0];
            pqdf.push(m[i][1] - m[i][0]);
        }
        for (size_t i = k0; i < n; ++i) {
            pqr1.push(m[i][1]);
            sr1 += m[i][1];
            if (pqr1.size() > k1) sr1 -= pqr1.top(), pqr1.pop();
        }
        int best_i = -1;
        T res = sr0 + sdf + sr1;
        for (size_t i = k0; i < k0 + k1; ++i) {
            sr0 += m[i][0];
            pqdf.push(m[i][1] - m[i][0]);
            sdf += pqdf.top();
            pqdf.pop();
            if (m[i][1] >= pqr1.top()) {
                sr1 -= m[i][1];
                pqkillr1.push(m[i][1]);
            } else {
                sr1 -= pqr1.top();
                pqr1.pop();
            }
            while (pqkillr1.size() && pqkillr1.top() == pqr1.top()) {
                pqkillr1.pop();
                pqr1.pop();
            }
            if (sr0 + sdf + sr1 > res) {
                res = sr0 + sdf + sr1;
                best_i = i;
            }
        }
        return res;
    }
};
