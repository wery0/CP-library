template<typename T>
class fenwick_seg_add {
    int n;
    vector<T> fen;

public:
    fenwick_seg_add() = default;
    fenwick_seg_add(int n): n(n + 1), fen(n + 1) {}
    template<typename I>
    fenwick_seg_add(I first, I last, bool init_with_pref_sums = false): n(std::distance(first, last) + 1), fen(n) {
        auto it = last;
        T sm = accumulate(first, last, T(0));
        for (int i = n - 1; i; --i, sm -= *it) {
            --it;
            T x = init_with_pref_sums ? sm : *it;
            fen[i] += x;
            fen[i - 1] -= x;
            fen[i - (i & -i)] += fen[i];
        }
    }

    void clear() {
        fill(fen.begin(), fen.end(), 0);
    }

    T operator[](int p) const {
        assert(0 <= p && p < n - 1);
        T res = 0;
        for (++p; p < n; p += p & -p) res += fen[p];
        return res;
    }

    void pref_add(int p, T x) {
        assert(-1 <= p && p < n - 1);
        for (++p; p; p -= p & -p) fen[p] += x;
    }

    void suf_add(int p, T x) {
        pref_add(n - 2, x);
        pref_add(p - 1, -x);
    }

    void seg_add(int l, int r, T x) {
        pref_add(r, x);
        pref_add(l - 1, -x);
    }
};
