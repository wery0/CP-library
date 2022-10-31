template<typename T>
struct segtree_point_upd {

    static const T inf = numeric_limits<T>::max();

    struct tag {

        T mx_pref = 0;
        T mx_suf = 0;
        T mx_seg = 0;
        T sm = 0, mx = -inf;

        tag() = default;

        friend inline void merge(const tag &l, const tag &r, tag &res) {
            res.sm = l.sm + r.sm;
            res.mx = max(l.mx, r.mx);
            res.mx_pref = max(l.mx_pref, l.sm + r.mx_pref);
            res.mx_suf = max(r.mx_suf, r.sm + l.mx_suf);
            res.mx_seg = max({l.mx_seg, r.mx_seg, l.mx_suf + r.mx_pref});
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag &l, const tag &r) {
            chmax(l.mx_seg, max(r.mx_seg, l.mx_suf + r.mx_pref));
            chmax(l.mx_pref, l.sm + r.mx_pref);
            l.mx_suf = max(r.mx_suf, r.sm + l.mx_suf);
            l.sm += r.sm;
            chmax(l.mx, r.mx);
        }

        friend inline void merge_to_right(const tag &l, tag &r) {
            chmax(r.mx_seg, max(l.mx_seg, l.mx_suf + r.mx_pref));
            chmax(r.mx_suf, r.sm + l.mx_suf);
            r.mx_pref = max(l.mx_pref, l.sm + r.mx_pref);
            r.sm += l.sm;
            chmax(r.mx, l.mx);
        }
    };

    tag neutral_tag;

    uint a, U;
    vec<tag> m;

    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        m.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            tag &t = m[U + q];
            const T val = *(first + q);
            t.sm = t.mx = val;
            t.mx_pref = t.mx_suf = t.mx_seg = max(val, (T)0);
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral<U>::value) {
            vec<T> m(n);
            (*this) = segtree_point_upd<T>(all(m));
        } else {
            (*this) = segtree_point_upd<T>(all(n));
        }
    }

    inline void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    inline T seg_subseg_w_max_sum(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        merge_to_left(lt, rt);
        return lt.mx < 0 ? lt.mx : lt.mx_seg;
    }

    inline void change(uint p, T nw) {
        p += U;
        m[p].sm = m[p].mx = nw;
        m[p].mx_pref = m[p].mx_suf = m[p].mx_seg = max((T)0, nw);
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//If all elements in array < 0, returns max element in array, not 0.