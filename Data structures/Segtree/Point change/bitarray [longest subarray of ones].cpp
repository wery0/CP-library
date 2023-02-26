class segtree_point_upd {

    struct tag {

        int cl = 0, cr = 0;
        int mxd = 0;
        int sz = 0;

        tag() = default;


        friend inline void merge(const tag& l, const tag& r, tag& res) {
            res.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            res.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            res.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            res.sz = l.sz + r.sz;
        }

        friend inline tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag& l, const tag& r) {
            l.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            l.cl += l.cl == l.sz ? r.cl : 0;
            l.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            l.sz += r.sz;
        }

        friend inline void merge_to_right(const tag& l, tag& r) {
            r.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            r.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            r.cr += + (r.cr == r.sz ? l.cr : 0);
            r.sz += l.sz;
        }
    };

    tag neutral_tag;

    size_t n, U;
    vector<tag> m;

    inline void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

public:
    segtree_point_upd() = default;

    template<typename T>
    segtree_point_upd(vector<T>& arr): n(arr.size()), U(n & (n - 1) ? 2 << __lg(n) : n), m(U * 2) {
        if (!n) return;
        for (const auto& i : arr) assert(0 <= i && i <= 1);
        for (size_t q = 0; q < n; ++q) {
            tag& t = m[U + q];
            t.cl = t.cr = arr[q];
            t.sz = 1;
        }
        for (size_t q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }

    segtree_point_upd(int n) {
        vector<bool> m(n);
        (*this) = segtree_point_upd(m);
    }

    tag query(size_t ql, size_t qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) merge_to_left(lt, m[ql]);
            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        return merge(lt, rt);
    }

    void change(size_t pos, int val) {
        assert(0 <= val && val <= 1);
        pos += U;
        m[pos].cl = m[pos].cr = m[pos].mxd = val;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};