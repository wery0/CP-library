template<typename T>
struct segtree {

    static constexpr T INF = numeric_limits<T>::max();

    uint a, U;
    vec<T> sm, mn, mx;
    vec<T> ps_add;

    segtree() = default;

    template<typename I>
    segtree(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        sm.resize(U * 2);
        mn.resize(U * 2, INF);
        mx.resize(U * 2, -INF);
        ps_add.resize(U * 2, 0);
        for (uint q = 0; q < a; q++) {
            const T val = *(first + q);
            sm[U + q] = val;
            mn[U + q] = val;
            mx[U + q] = val;
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

    void apply_add(int v, T val) {
        ps_add[v] += val;
        sm[v] += val * gsz(v);
        mn[v] += val;
        mx[v] += val;
    }

    void push(uint v) {
        if (ps_add[v] != 0) {
            apply_add(v << 1, ps_add[v]);
            apply_add(v << 1 | 1, ps_add[v]);
            ps_add[v] = 0;
        }
    }

    void upd(uint v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

    T seg_max(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return -INF;
        if (ql <= l && r <= qr) {
            return mx[v];
        }
        push(v);
        uint md = (l + r) >> 1;
        return max(seg_max(ql, qr, l, md, v << 1),
                   seg_max(ql, qr, md + 1, r, v << 1 | 1));
    }

    T seg_min(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) {
            return mn[v];
        }
        push(v);
        uint md = (l + r) >> 1;
        return min(seg_min(ql, qr, l, md, v << 1),
                   seg_min(ql, qr, md + 1, r, v << 1 | 1));
    }

    T seg_sum(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return sm[v];
        }
        push(v);
        uint md = (l + r) >> 1;
        return seg_sum(ql, qr, l, md, v << 1) +
               seg_sum(ql, qr, md + 1, r, v << 1 | 1);
    }

    void seg_add(uint ql, uint qr, uint l, uint r, uint v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, val);
            return;
        }
        push(v);
        uint md = (l + r) >> 1;
        seg_add(ql, qr, l, md, v << 1, val);
        seg_add(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

    T operator[](uint pos) {
        uint l = 0, r = U - 1, v = 1;
        uint res = 0;
        while (l != r) {
            res += ps_add[v];
            uint md = (l + r) >> 1;
            if (pos <= md) {
                r = md;
                v = v << 1;
            } else {
                l = md + 1;
                v = v << 1 | 1;
            }
        }
        return res + sm[v];
    }

    void get_last_layer(vec<T> &res) {
        for (int q = 1; q < U; q++) push(q);
        for (int q = 0; q < res.size(); ++q) {
            res[q] = sm[U + q];
        }
    }

    inline void seg_add(uint ql, uint qr, T val) { seg_add(ql, qr, 0, U - 1, 1, val); }
    T seg_sum(uint ql, uint qr) { return seg_sum(ql, qr, 0, U - 1, 1); }
    T seg_min(uint ql, uint qr) { return seg_min(ql, qr, 0, U - 1, 1); }
    T seg_max(uint ql, uint qr) { return seg_max(ql, qr, 0, U - 1, 1); }
};