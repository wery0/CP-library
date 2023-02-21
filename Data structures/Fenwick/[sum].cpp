template<typename T>
class fenwick {
    int n;
    vector<T> fen;

public:
    fenwick() = default;
    fenwick(int n): n(n + 1), fen(n + 1) {}
    template<typename I>
    fenwick(I first, I last): n(last - first + 1), fen(n) {
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
        fill(fen.begin(), fen.end(), 0);
    }

    void point_add(int p, T x) {
        assert(0 <= p && p < n - 1);
        for (++p; p < n; p += p & -p) fen[p] += x;
    }

    T pref_sum(int p) const {
        assert(-1 <= p && p < n - 1);
        T ans = 0;
        for (++p; p; p -= p & -p) ans += fen[p];
        return ans;
    }

    T suf_sum(int p) const {
        return pref_sum(n - 2) - pref_sum(p - 1);
    }

    T seg_sum(int l, int r) const {
        return pref_sum(r) - pref_sum(l - 1);
    }

    //[1, 2, 3] -> f(0) = -1, f(1) = 0, f(4) = 1, f(6) = 2
    int max_pref_with_sum_leq_k(T k) const {
        int p = 0, step = 1 << __lg(n);
        T s = 0;
        for (; step; step >>= 1) {
            int nw = p + step;
            if (nw < n && s + fen[nw] <= k) p = nw, s += fen[nw];
        }
        return p - 1;
    }
};