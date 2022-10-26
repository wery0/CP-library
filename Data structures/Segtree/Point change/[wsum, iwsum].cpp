template<typename T>
struct segtree_point_upd {

    struct tag {

        T sm = 0;
        T wsm = 0;
        T iwsm = 0;
        T len = 0;

        tag() = default;

        friend inline void merge(const tag &l, const tag &r, tag &res) {
            res.len = l.len + r.len;
            res.sm = l.sm + r.sm;
            res.wsm = l.wsm + r.wsm + r.sm * l.len;
            res.iwsm = r.iwsm + l.iwsm + l.sm * r.len;
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag &l, const tag &r) {
            l.wsm += r.wsm + r.sm * l.len;
            l.iwsm += r.iwsm + l.sm * r.len;
            l.sm += r.sm;
            l.len += r.len;
        }

        friend inline void merge_to_right(const tag &l, tag &r) {
            r.wsm += l.wsm + r.sm * l.len;
            r.iwsm += l.iwsm + l.sm * r.len;
            r.sm += l.sm;
            r.len += l.len;
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
            t.sm = val;
            t.wsm = val;
            t.iwsm = val;
            t.len = 1;
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

    //sum{i = l}{r}{arr[i] * (i - l + 1)}
    inline T seg_wsum(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        merge_to_left(lt, rt);
        return lt.wsm;
    }

    //sum{i = l}{r}{arr[i] * (r - i + 1)}
    inline T seg_iwsum(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        merge_to_left(lt, rt);
        return lt.iwsm;
    }

    inline void point_change(uint p, T val) {
        p += U;
        m[p].sm = m[p].wsm = m[p].iwsm = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//Bottom-up segtree, supports only point updates