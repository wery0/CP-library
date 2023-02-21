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
    fenwick(const T_arr& m, typename enable_if<!is_integral_v<T_arr >>::type* = 0) {
        (*this) = fenwick(m.begin(), m.end());
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