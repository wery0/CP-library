template<typename T>
class fenwick {
    int n;
    T sum_all = 0;
    vector<T> fen;

public:
    fenwick() = default;
    fenwick(int n): n(n + 1), fen(n + 1) {}
    template<typename I>
    fenwick(I first, I last): n(std::distance(first, last) + 1), fen(n), sum_all(accumulate(first, last, T(0))) {
        auto it = first;
        for (int i = 1; i < n; ++i, ++it) {
            fen[i] += *it;
            const int nw = i + (i & -i);
            if (nw < n) fen[nw] += fen[i];
        }
    }
    template<typename T_arr>
    fenwick(const T_arr& m, typename enable_if<!is_integral_v<T_arr>>::type* = 0) {
        (*this) = fenwick(m.begin(), m.end());
    }

    void clear() {
        sum_all = 0;
        fill(fen.begin(), fen.end(), 0);
    }

    void point_add(int p, T x) {
        assert(0 <= p && p < n - 1);
        sum_all += x;
        for (++p; p < n; p += p & -p) fen[p] += x;
    }

    T pref_sum(int p) const {
        assert(-1 <= p && p < n - 1);
        T ans = 0;
        for (++p; p; p -= p & -p) ans += fen[p];
        return ans;
    }

    T suf_sum(int p) const {
        return sum_all - pref_sum(p - 1);
    }

    T seg_sum(int l, int r) const {
        return pref_sum(r) - pref_sum(l - 1);
    }

    //[1, 2, 3] -> f(0) = -1, f(1) = 0, f(4) = 1, f(6) = 2
    int max_pref_with_sum_leq_k(T k) const {
        int p = 0;
        for (int step = 1 << __lg(n); step; step >>= 1) {
            int nw = p + step;
            if (nw < n && fen[nw] <= k) p = nw, k -= fen[nw];
        }
        return p - 1;
    }

    //[1, 2, 3] -> f(2) = 3, f(3) = 2, f(5) = 1, f(6) = 0
    int max_suf_with_sum_leq_k(T k) const {
        T vse = sum_all;
        int p = 0;
        for (int step = 1 << __lg(n); step; step >>= 1) {
            int nw = p + step;
            if (nw < n && vse - fen[nw] > k) p = nw, vse -= fen[nw];
        }
        return p + (k < sum_all);
    }

    //Returns smallest l - 1 <= r <= n - 1 s. t. seg_sum(l, r) <= k
    int max_right_border_of_seg_with_sum_leq_k(int l, T k) const {
        assert(0 <= l && l < n - 1);
        return max_pref_with_sum_leq_k(pref_sum(l - 1) + k);
    }

    //Returns biggest 0 <= l <= r + 1 s. t. seg_sum(l, r) <= k
    int max_left_border_of_seg_with_sum_leq_k(int r, T k) const {
        assert(0 <= r && r < n - 1);
        return max_suf_with_sum_leq_k(k + suf_sum(r + 1));
    }
};
