// G must be at least 1 greater than the array size
template<typename T>
struct fenwick {
    int G;
    vec<T> fen;
    //T fen[G] = {0};
    //array<T, G> fen{};

    fenwick() {}

    fenwick(int _G) {
        G = _G;
        fen.resize(G);
    }

    fenwick(vec<T> &n) {
        G = n.size() + 1;
        fen.resize(G);
        for (int q = 1; q <= n.size(); q++) {
            fen[q] += n[q - 1];
            const int nw = q + (q & -q);
            if (nw < G) fen[nw] += fen[q];
        }
        for (int q = n.size() + 1; q < G; q++) {
            const int nw = q + (q & -q);
            if (nw < G) fen[nw] += fen[q];
        }
    }

    void clear() {
        fill(all(fen), 0);
    }

    T operator[](int p) {
        p += 1;
        assert(1 <= p && p < G);
        T o = 0;
        for (; p < G; p += p & -p) {
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
        assert(1 <= l && r < G);
        pref_add(r, x);
        pref_add(l - 1, -x);
    }
};