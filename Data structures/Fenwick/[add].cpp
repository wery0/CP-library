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
            seg_add(i - 1, i - 1, *it);
        }
    }
    template<typename T_arr>
    fenwick(const T_arr& m) {
        (*this) = fenwick(m.begin(), m.end());
    }

    void clear() {
        fill(fen.begin(), fen.end(), 0);
    }

    T operator[](int p) {
        ++p;
        assert(1 <= p && p < n);
        T res = 0;
        for (; p < n; p += p & -p) res += fen[p];
        return res;
    }

    void pref_add(int p, T x) {
        for (; p; p -= p & -p) fen[p] += x;
    }

    void seg_add(int l, int r, T x) {
        ++l, ++r;
        assert(1 <= l && r < n);
        pref_add(r, x);
        pref_add(l - 1, -x);
    }
};