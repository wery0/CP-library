template<typename T>
class segtree_point_upd {

    struct tag {

        T sm = 0;
        T wsm = 0;
        T iwsm = 0;
        T len = 0;

        tag() = default;

        friend inline void merge(const tag& l, const tag& r, tag& res) {
            res.len = l.len + r.len;
            res.sm = l.sm + r.sm;
            res.wsm = l.wsm + r.wsm + r.sm * l.len;
            res.iwsm = r.iwsm + l.iwsm + l.sm * r.len;
        }

        friend inline tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

        friend inline void merge_to_left(tag& l, const tag& r) {
            l.wsm += r.wsm + r.sm * l.len;
            l.iwsm += r.iwsm + l.sm * r.len;
            l.sm += r.sm;
            l.len += r.len;
        }

        friend inline void merge_to_right(const tag& l, tag& r) {
            r.wsm += l.wsm + r.sm * l.len;
            r.iwsm += l.iwsm + l.sm * r.len;
            r.sm += l.sm;
            r.len += l.len;
        }
    };

    tag neutral_tag;

    size_t n, U;
    vector<tag> m;

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
	m.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            tag& t = m[U + i];
            const T val = *(first + i);
            t.sm = val;
            t.wsm = val;
            t.iwsm = val;
            t.len = 1;
        }
        for (size_t i = U; --i;) {
            const tag &l = m[i << 1], &r = m[i << 1 | 1];
            merge(l, r, m[i]);
        }
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral<U>::value) {
            vector<T> m(n);
            (*this) = segtree_point_upd<T>(m.begin(), m.end());
        } else {
            (*this) = segtree_point_upd<T>(n.begin(), n.end());
        }
    }

    //sum{i = l}{r}{arr[i] * (i - l + 1)}
    T seg_wsum(size_t ql, size_t qr) const {
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
    T seg_iwsum(size_t ql, size_t qr) const {
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

    void point_change(size_t pos, T val) {
        pos += U;
        m[pos].sm = m[pos].wsm = m[pos].iwsm = val;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};