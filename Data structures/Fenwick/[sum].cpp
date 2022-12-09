template<typename T>
struct fenwick {
    int N;
    vector<T> fen;

    fenwick() = default;
    fenwick(int N): N(N + 1), fen(N + 1) {}
    fenwick(vector<T> &n): N(n.size() + 1), fen(N) {
        for (int q = 1; q <= n.size(); q++) {
            fen[q] += n[q - 1];
            const int nw = q + (q & -q);
            if (nw < N) fen[nw] += fen[q];
        }
        for (int q = n.size() + 1; q < N; q++) {
            const int nw = q + (q & -q);
            if (nw < N) fen[nw] += fen[q];
        }
    }

    void clear() {
        fill(all(fen), 0);
    }

    void point_add(int p, T x) {
        assert(0 <= p && p < N - 1);
        for (p += 1; p < N; p += p & -p) fen[p] += x;
    }

    T pref_sum(int p) {
        assert(-1 <= p && p < N - 1);
        T o = 0;
        for (p += 1; p; p -= p & -p) o += fen[p];
        return o;
    }

    T suf_sum(int p) {
        return pref_sum(N - 2) - pref_sum(p - 1);
    }

    T seg_sum(int l, int r) {
        assert(0 <= l && r < N - 1);
        return pref_sum(r) - pref_sum(l - 1);
    }

    //[1, 2, 3] -> f(0) = -1, f(1) = 1, f(4) = 1, f(6) = 2
    int max_pref_with_sum_leq_k(T k) {
        int p = 0, step = 1 << __lg(N);
        T s = 0;
        for (; step; step >>= 1) {
            int nw = p + step;
            if (nw < N && s + fen[nw] <= k) p = nw, s += fen[nw];
        }
        return p - 1;
    }
};