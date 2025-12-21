template<typename T>
class segtree_point_upd {

    static constexpr T inf = numeric_limits<T>::max();

    struct tag {

        T mx_pref = 0;
        T mx_suf = 0;
        T mx_seg = 0;
        T sm = 0, mx = -inf;

        tag() = default;

        friend void merge(const tag& l, const tag& r, tag& res) {
            res.sm = l.sm + r.sm;
            res.mx = max(l.mx, r.mx);
            res.mx_pref = max(l.mx_pref, l.sm + r.mx_pref);
            res.mx_suf = max(r.mx_suf, r.sm + l.mx_suf);
            res.mx_seg = max({l.mx_seg, r.mx_seg, l.mx_suf + r.mx_pref});
        }

        friend tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend void merge_to_left(tag& l, const tag& r) {
            l.mx_seg = max(l.mx_seg, max(r.mx_seg, l.mx_suf + r.mx_pref));
            l.mx_pref = max(l.mx_pref, l.sm + r.mx_pref);
            l.mx_suf = max(r.mx_suf, r.sm + l.mx_suf);
            l.sm += r.sm;
            l.mx = max(l.mx, r.mx);
        }

        friend void merge_to_right(const tag& l, tag& r) {
            r.mx_seg = max(r.mx_seg, max(l.mx_seg, l.mx_suf + r.mx_pref));
            r.mx_suf = max(r.mx_suf, r.sm + l.mx_suf);
            r.mx_pref = max(l.mx_pref, l.sm + r.mx_pref);
            r.sm += l.sm;
            r.mx = max(r.mx, l.mx);
        }
    };

    tag neutral_tag;

    size_t n;
    vector<tag> m;

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    void init_tag_by_value(tag& t, const T& val) {
        t.sm = t.mx = val;
        t.mx_pref = t.mx_suf = t.mx_seg = max(val, (T)0);
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(std::distance(first, last)) {
        if (!n) return;
        m.resize(n * 2);
        for (size_t i = 0; i < n; ++i, ++first) {
            init_tag_by_value(m[n + i], *first);
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

    //If all elements in segment < 0, returns max element in segment, not 0.
    T seg_subsegment_with_max_sum(size_t l, size_t r) const {
        l += n, r += n;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) merge_to_left(lt, m[l]);
            if (~r & 1) merge_to_right(m[r], rt);
        }
        merge_to_left(lt, rt);
        return lt.mx < 0 ? lt.mx : lt.mx_seg;
    }

    void point_change(size_t pos, T val) {
        pos += n;
        init_tag_by_value(m[pos], val);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
