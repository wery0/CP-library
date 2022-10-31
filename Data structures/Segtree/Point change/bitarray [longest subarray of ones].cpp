struct segtree_point_upd {

    struct tag {

        int cl = 0, cr = 0;
        int mxd = 0;
        int sz = 0;

        tag() = default;


        friend inline void merge(const tag &l, const tag &r, tag &res) {
            res.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            res.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            res.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            res.sz = l.sz + r.sz;
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag &l, const tag &r) {
            l.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            l.cl += l.cl == l.sz ? r.cl : 0;
            l.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            l.sz += r.sz;
        }

        friend inline void merge_to_right(const tag &l, tag &r) {
            r.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            r.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            r.cr += + (r.cr == r.sz ? l.cr : 0);
            r.sz += l.sz;
        }
    };

    tag neutral_tag;

    uint a, U;
    vec<tag> m;

    segtree_point_upd() = default;

    template<typename T>
    segtree_point_upd(vec<T> &n) {
        assert(0 <= *min_element(all(n)) && *max_element(all(n)) <= 1);
        a = n.size();
        U = geq_pow2(a);
        m.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            tag &t = m[U + q];
            t.cl = t.cr = n[q];
            t.sz = 1;
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }

    segtree_point_upd(int a) {
        vec<bool> m(a);
        (*this) = segtree_point_upd(m);
    }

    inline void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    inline tag query(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        return merge(lt, rt);
    }

    inline void change(uint p, int val) {
        assert(0 <= val && val <= 1);
        p += U;
        m[p].cl = m[p].cr = m[p].mxd = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};