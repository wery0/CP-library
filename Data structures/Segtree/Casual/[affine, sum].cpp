template<typename T>
class segtree {

    size_t a, U;
    vector<T> sm;
    vector<T> psa;
    vector<T> psb;

    inline constexpr int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    inline void apply(int v, T a, T b) {
        psa[v] *= a;
        psb[v] = psb[v] * a + b;
        sm[v] = sm[v] * a +  gsz(v) * b;
    }

    inline void push(size_t v) {
        if (psa[v] == 1 && psb[v] == 0) return;
        apply(v << 1, psa[v], psb[v]);
        apply(v << 1 | 1, psa[v], psb[v]);
        psa[v] = 1;
        psb[v] = 0;
    }

    inline void upd(size_t v) {
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

    void seg_affine(size_t ql, size_t qr, size_t l, size_t r, size_t v, T va, T vb) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(v, va, vb);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_affine(ql, qr, l, md, v << 1, va, vb);
        seg_affine(ql, qr, md + 1, r, v << 1 | 1, va, vb);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): a(last - first), U(geq_pow2(a)) {
        sm.resize(U * 2);
        psa.resize(U * 2, 1);
        psb.resize(U * 2);
        for (size_t q = 0; q < a; q++) {
            const T val = *(first + q);
            sm[U + q] = val;
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

    T seg_sum(size_t ql, size_t qr) {return seg_sum(ql, qr, 0, U - 1, 1);}
    inline void seg_affine(size_t ql, size_t qr, T va, T vb) { seg_affine(ql, qr, 0, U - 1, 1, va, vb);}
};