class segtree_point_upd {

    struct tag {

        int cl = 0, cr = 0;
        int mxd = 0;
        int sz = 0;

        tag() = default;

        friend void merge(const tag& l, const tag& r, tag& res) {
            res.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            res.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            res.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            res.sz = l.sz + r.sz;
        }

        friend tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend void merge_to_left(tag& l, const tag& r) {
            l.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            l.cl += l.cl == l.sz ? r.cl : 0;
            l.cr = r.cr + (r.cr == r.sz ? l.cr : 0);
            l.sz += r.sz;
        }

        friend void merge_to_right(const tag& l, tag& r) {
            r.mxd = max({l.mxd, r.mxd, l.cr + r.cl});
            r.cl = l.cl + (l.cl == l.sz ? r.cl : 0);
            r.cr += r.cr == r.sz ? l.cr : 0;
            r.sz += l.sz;
        }
    };

    tag neutral_tag;

    size_t n;
    vector<tag> m;

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    void init_tag_by_value(tag& t, int val) {
        t.cl = t.cr = t.mxd = val;
        t.sz = 1;
    }

public:
    segtree_point_upd() = default;

    template<typename T>
    segtree_point_upd(vector<T>& arr): n(arr.size()) {
        if (!n) return;
        for (const auto& i : arr) assert(0 <= i && i <= 1);
        m.resize(n * 2);
        for (size_t i = 0; i < n; ++i) {
            init_tag_by_value(m[n + i], arr[i]);
        }
        for (size_t i = n; --i;) {
            const tag& l = m[i << 1], &r = m[i << 1 | 1];
            merge(l, r, m[i]);
        }
    }

    segtree_point_upd(size_t n) {
        vector<bool> m(n);
        (*this) = segtree_point_upd(m);
    }

    tag query(size_t l, size_t r) const {
        l += n, r += n;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) merge_to_left(lt, m[l]);
            if (~r & 1) merge_to_right(m[r], rt);
        }
        return merge(lt, rt);
    }

    void point_change(size_t pos, int val) {
        assert(0 <= val && val <= 1);
        pos += n;
        init_tag_by_value(m[pos], val);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
