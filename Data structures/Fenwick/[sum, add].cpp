template<typename T>
class fenwick_seg_sum_add {

    struct fenwick_seg_add {
        int n;
        vector<T> fen;

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
    };

    int n;
    fenwick_seg_add f1, f2;
    T sum_all = 0;

public:
    fenwick_seg_sum_add() = default;
    fenwick_seg_sum_add(int n): n(n), f1(n), f2(n) {}
    template<typename I>
    fenwick_seg_sum_add(I first, I last): n(std::distance(first, last)), f1(n), f2(first, last, true), sum_all(accumulate(first, last, T(0))) {}

    void clear() {sum_all = 0; f1.clear(); f2.clear();}

    void pref_add(int p, T x) {sum_all += x * (p + 1); f1.pref_add(p, x); f2.suf_add(p + 1, (p + 1) * x);}
    void suf_add(int p, T x) {pref_add(n - 1, x); pref_add(p - 1, -x);}
    T pref_sum(int p) const {return f1[p] * (p + 1) + f2[p];}
    T suf_sum(int p) const {return sum_all - pref_sum(p - 1);}

    void seg_add(int l, int r, T x) {pref_add(r, x); pref_add(l - 1, -x);}
    T seg_sum(int l, int r) const {return pref_sum(r) - (l ? pref_sum(l - 1) : 0);}
    T operator[](int p) const {return seg_sum(p, p);}
};
