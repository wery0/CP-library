template<typename T>
class segtree {

    size_t a, U;
    vector<T> sm;
    vector<T> psA;
    vector<T> psB;

    inline constexpr int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply_add(int v, T a, T b) {
        psA[v] += a;
        psB[v] += b;
        sm[v] += a * gsz(v) * gsz(v) + gsz(v) * b;
    }

    void push(size_t v) {
        apply_add(v << 1, psA[v], psB[v]);
        apply_add(v << 1 | 1, psA[v], psA[v] * gsz(v << 1) * 2 + psB[v]);
        psA[v] = psB[v] = 0;
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
    }

    T seg_sum(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return sm[v];
        }
        push(v);
        size_t md = (l + r) >> 1;
        const auto lf = seg_sum(ql, qr, l, md, v << 1);
        const auto rg = seg_sum(ql, qr, md + 1, r, v << 1 | 1);
        return lf + rg;
    }

    void seg_add_progression(size_t ql, size_t qr, size_t l, size_t r, size_t v, T a0, T b) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, b, 2 * a0 - b);
            return;
        }
        push(v);
        int md = (l + r) >> 1;
        seg_add_progression(ql, qr, l, md, v << 1, a0, b);
        int llen = max(0, (int)(md - max(ql, l) + 1));
        seg_add_progression(ql, qr, md + 1, r, v << 1 | 1, a0 + llen * b, b);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): a(last - first), U(geq_pow2(a)) {
        sm.resize(U * 2);
        psA.resize(U * 2);
        psB.resize(U * 2);
        for (size_t q = 0; q < a; q++) {
            const T val = *(first + q);
            sm[U + q] = val * 2;
        }
        for (size_t q = U; --q;) {
            upd(q);
        }
    }

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral<U>::value) {
            vector<T> m(n);
            (*this) = segtree<T>(m.begin(), m.end());
        } else {
            (*this) = segtree<T>(n.begin(), n.end());
        }
    }

    T operator[](size_t pos) {
        size_t l = 0, r = U - 1, v = 1;
        T o = 0;
        while (l != r) {
            push(v);
            size_t md = (l + r) >> 1;
            if (pos <= md) {
                r = md;
                v = v << 1;
            } else {
                l = md + 1;
                v = v << 1 | 1;
            }
        }
        return sm[v];
    }

    void get_last_layer(vector<T>& res) {
        for (int q = 1; q < U; q++) push(q);
        for (int q = 0; q < res.size(); ++q) {
            res[q] = sm[U + q];
        }
    }

    T seg_sum(size_t ql, size_t qr) {return seg_sum(ql, qr, 0, U - 1, 1) / 2;}
    inline void seg_add_progression(size_t ql, size_t qr, T a0, T b) {seg_add_progression(ql, qr, 0, U - 1, 1, a0, b);}
};