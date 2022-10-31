const ll mod = 998244353;
template<typename T>
struct segtree {

    uint a, U;
    vec<T> sm;
    vec<T> psa;
    vec<T> psb;

    segtree() = default;

    template<typename I>
    segtree(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        sm.resize(U * 2);
        psa.resize(U * 2, 1);
        psb.resize(U * 2);
        for (uint q = 0; q < a; q++) {
            const T val = *(first + q);
            sm[U + q] = val;
        }
        for (uint q = U; --q;) {
            upd(q);
        }
    }

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral<U>::value) {
            vec<T> m(n);
            (*this) = segtree<T>(all(m));
        } else {
            (*this) = segtree<T>(all(n));
        }
    }

    inline constexpr int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    inline void apply(int v, T a, T b) {
        psa[v] = (1ull * psa[v] * a) % mod;
        psb[v] = (1ull * psb[v] * a + b) % mod;
        sm[v] = (1ull * sm[v] * a + 1ull * gsz(v) * b) % mod;
    }

    inline void push(uint v) {
        if (psa[v] == 1 && psb[v] == 0) return;
        apply(v << 1, psa[v], psb[v]);
        apply(v << 1 | 1, psa[v], psb[v]);
        psa[v] = 1;
        psb[v] = 0;
    }

    inline void upd(uint v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        if (sm[v] >= mod) sm[v] -= mod;
    }

    T seg_sum(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return sm[v];
        }
        push(v);
        uint md = (l + r) >> 1;
        const auto lf = seg_sum(ql, qr, l, md, v << 1);
        const auto rg = seg_sum(ql, qr, md + 1, r, v << 1 | 1);
        return (lf + rg) % mod;
    }

    void seg_affine(uint ql, uint qr, uint l, uint r, uint v, T va, T vb) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(v, va, vb);
            return;
        }
        push(v);
        uint md = (l + r) >> 1;
        seg_affine(ql, qr, l, md, v << 1, va, vb);
        seg_affine(ql, qr, md + 1, r, v << 1 | 1, va, vb);
        upd(v);
    }

    T operator[](uint pos) {
        uint l = 0, r = U - 1, v = 1;
        T o = 0;
        while (l != r) {
            push(v);
            uint md = (l + r) >> 1;
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

    void get_last_layer(vec<T> &res) {
        for (int q = 1; q < U; q++) push(q);
        for (int q = 0; q < res.size(); ++q) {
            res[q] = sm[U + q];
        }
    }

    T seg_sum(uint ql, uint qr) { return seg_sum(ql, qr, 0, U - 1, 1) % mod; }
    inline void seg_affine(uint ql, uint qr, T va, T vb) { seg_affine(ql, qr, 0, U - 1, 1, va, vb); }
};