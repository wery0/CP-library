template<typename T>
struct segtree {

    struct tag {

        tag() = default;

        friend inline void push_tag(tag &t, tag &l, tag &r) {
            //write push if need, or remove it
        }

        friend inline bool is_neutral(const tag &t) {
            //write is_neutral if need, or remove it
            return false;
        }

        friend inline void merge(const tag &l, const tag &r, tag &res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //write merge
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }
    };

    tag neutral_tag;   //set neutral tag

    uint a, U;
    vec<tag> m;

    segtree() = default;

    template<typename I>
    segtree(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        m.resize(U * 2);
        for (uint q = 0; q < a; q++) {
            tag &t = m[U + q];
            const T val = *(first + q);
            //write init of last layer
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }

    template<typename T_arr>
    segtree(T_arr &n) {
        (*this) = segtree<T>(all(n));
    }

    segtree(uint a) {
        vec<T> m(a);
        (*this) = segtree<T>(m);
    }

    inline constexpr int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void push(uint v) {
        push_tag(m[v], m[v << 1], m[v << 1 | 1]);
    }

    void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    tag seg_statistic(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return neutral_tag;
        if (ql <= l && r <= qr) {
            return m[v];
        }
        push(v);
        uint md = l + (r - l) >> 1;
        const tag lf = seg_statistic(ql, qr, l, md, v << 1);
        const tag rg = seg_statistic(ql, qr, md + 1, r, v << 1 | 1);
        return merge(lf, rg);
    }
    tag seg_statistic(uint ql, uint qr) { return seg_statistic(ql, qr, 0, U - 1, 1); }

    void seg_change(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //write change function
            return;
        }
        push(v);
        uint md = l + (r - l) >> 1;
        seg_change(ql, qr, l, md, v << 1);
        seg_change(ql, qr, md + 1, r, v << 1 | 1);
        upd(v);
    }
    inline void seg_change(uint ql, uint qr) { seg_change(ql, qr, 0, U - 1, 1); }
};