// N must be at least 1 greater than the array size
template<typename T>
struct fenwick {
    int N;
    vec<T> fen;
    //T fen[N] = {0};
    //array<T, N> fen{};

    fenwick() = default;

    fenwick(int _N) {
        N = _N;
        fen.resize(N);
    }

    fenwick(vec<T> &n) {
        N = n.size() + 1;
        fen.resize(N);
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

    T operator[](int p) {
        p += 1;
        assert(1 <= p && p < N);
        T o = 0;
        for (; p < N; p += p & -p) {
            o += fen[p];
        }
        return o;
    }

    void pref_add(int p, T x) {
        for (; p; p -= p & -p) {
            fen[p] += x;
        }
    }

    void seg_add(int l, int r, T x) {
        l += 1, r += 1;
        assert(1 <= l && r < N);
        pref_add(r, x);
        pref_add(l - 1, -x);
    }
};