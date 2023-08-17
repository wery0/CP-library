template<typename T>
class segtree_point_upd {

    struct tag {

    	T lc, rc;
        int dl = 0, dr = 0;
        int mxd = 0;
        int sz = 0;

        tag() = default;

        friend bool is_neutral(const tag& t) {
            return t.sz == 0;
        }

        friend void merge(const tag& l, const tag& r, tag& res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
			res.lc = l.lc;
            res.rc = r.rc;
            res.dl = l.dl + (l.dl == l.sz && l.rc == r.lc ? r.dl : 0);
            res.dr = r.dr + (r.dr == r.sz && l.rc == r.lc ? l.dr : 0);
            res.mxd = max({l.mxd, r.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)});
            res.sz = l.sz + r.sz;
        }

        friend tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

		friend void merge_to_left(tag& l, const tag& r) {
			if (is_neutral(l)) {l = r; return;}if (is_neutral(r)) return;
			l.mxd = max({l.mxd, r.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)});
			l.dl += l.dl == l.sz && r.lc == l.lc ? r.dl : 0;
			l.dr = r.dr + (r.dr == r.sz && l.rc == r.lc ? l.dr : 0);
			l.rc = r.rc;
		}

		friend void merge_to_right(const tag& l, tag& r) {
		    if (is_neutral(l)) return;if (is_neutral(r)) {r = l; return;}
		    r.mxd = max({r.mxd, l.mxd, (l.rc == r.lc ? l.dr + r.dl : 0)});
		    r.dl = l.dl + (l.dl == l.sz && r.lc == l.rc ? r.dl : 0);
		    r.dr += r.dr == r.sz && l.rc == r.lc ? l.dr : 0;
		    r.lc = l.lc;
		}
    };

    tag neutral_tag;

    size_t n;
    vector<tag> m;

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    void init_tag_by_value(tag& t, const T& val) {
		t.lc = t.rc = val;
        t.dl = t.dr = t.mxd = t.sz = 1;
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): n(last - first) {
        if (!n) return;
        m.resize(n * 2);
        for (size_t i = 0; i < n; ++i) {
        	init_tag_by_value(m[n + i], *(first + i));
        }
        for (size_t i = n; --i;) {
            const tag& l = m[i << 1], &r = m[i << 1 | 1];
            merge(l, r, m[i]);
        }
    }
    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree_point_upd<T>(m.begin(), m.end());
        } else {
            (*this) = segtree_point_upd<T>(n.begin(), n.end());
        }
    }

    tag seg_longest_substring_of_equal_elements(size_t l, size_t r) const {
        l += n, r += n;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
           if (l & 1) merge_to_left(lt, m[l]);
           if (~r & 1) merge_to_right(m[r], rt);
        }
        return merge(lt, rt);
    }

    void point_change(size_t pos, T val) {
        pos += n;
        init_tag_by_value(m[pos], val);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//Bottom-up segtree, supports only point updates
