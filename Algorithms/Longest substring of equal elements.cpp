template<typename T>
struct segtree_point_upd {

    struct tag {

        T lc, rc;
        int dl = 0, dr = 0;
        int mxd = 0;
        int sz = 0;

        tag() {}

        friend inline bool is_neutral(const tag &t) {
            return t.sz == 0;
        }

        friend inline void merge(const tag &l, const tag &r, tag &res) {
            if (is_neutral(l)) {res = r; return;}
            if (is_neutral(r)) {res = l; return;}
            res.lc = l.lc;
            res.rc = r.rc;
            res.dl = l.dl + (l.dl == l.sz && l.rc == r.lc ? r.dl : 0);
            res.dr = r.dr + (r.dr == r.sz && l.rc == r.lc ? l.dr : 0);
            res.mxd = max({l.mxd, r.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)});
            res.sz = l.sz + r.sz;
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag &l, const tag &r) {
            if (is_neutral(l)) {l = r; return;}
            if (is_neutral(r)) return;
            chmax(l.mxd, max(r.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)));
            l.dl += l.dl == l.sz && r.lc == l.lc ? r.dl : 0;
            l.dr = r.dr + (r.dr == r.sz && l.rc == r.lc ? l.dr : 0);
            l.rc = r.rc;
        }

        friend inline void merge_to_right(const tag &l, tag &r) {
            if (is_neutral(l)) return;
            if (is_neutral(r)) {r = l; return;}
            chmax(r.mxd, max(l.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)));
            r.dl = l.dl + (l.dl == l.sz && r.lc == l.rc ? r.dl : 0);
            r.dr += r.dr == r.sz && l.rc == r.lc ? l.dr : 0;
            r.lc = l.lc;
        }
    };

    tag neutral_tag;

    uint U;
    vec<tag> m;
    segtree_point_upd() {}

    template<typename I>
    segtree_point_upd(I fir, I last) {
        int a = last - fir;
        U = geq_pow2(a);
        m.resize(U * 2);
        for (uint q = 0; q < a; ++q, ++fir) {
            tag &t = m[U + q];
            t.lc = t.rc = *fir;
            t.dl = t.dr = t.mxd = t.sz = 1;
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }

    inline void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    inline tag query(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            //if (ql & 1) lt = merge(lt, m[ql]);
            //if (~qr & 1) rt = merge(m[qr], rt);
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        return merge(lt, rt);
    }

    inline void change(uint p, T nw) {
        p += U;
        m[p].lc = m[p].rc = nw;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};