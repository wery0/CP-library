template<typename T>
class segtree_point_upd {

    struct tag {

        tag() = default;

        friend inline bool is_neutral(const tag& t) {
            //Write neutral if need, or remove it
            return false;
        }

        friend inline void merge(const tag& l, const tag& r, tag& res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //Write merge
        }

        friend inline tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

//        friend inline void merge_to_left(tag& l, const tag& r) {
//            if (is_neutral(l)) {l = r; return;}if (is_neutral(r)) return;
//
//        }
//
//        friend inline void merge_to_right(const tag& l, tag& r) {
//            if (is_neutral(l)) return;if (is_neutral(r)) {r = l; return;}
//
//        }
    };

    tag neutral_tag;    //Init neutral tag

    size_t n, U;
    vector<tag> m;

    inline void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        m.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            tag& t = m[U + i];
            const T val = *(first + i);
            //Write init of last layer
        }
        for (size_t i = U; --i;) {
            const tag &l = m[i << 1], &r = m[i << 1 | 1];
            merge(l, r, m[i]);
        }
    }
    template<typename T_arr>
    segtree_point_upd(T_arr& n) {
        (*this) = segtree_point_upd<T>(n.begin(), n.end());
    }
    segtree_point_upd(size_t n) {
        vector<T> m(n);
        (*this) = segtree_point_upd<T>(m);
    }

    inline tag seg_statistic(size_t ql, size_t qr) const {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) lt = merge(lt, m[ql]);
            if (~qr & 1) rt = merge(m[qr], rt);
//            if (ql & 1) merge_to_left(lt, m[ql]);
//            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        return merge(lt, rt);
    }

    void point_change(size_t pos, tag t) {
        pos += U;
        m[pos] = t;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//Bottom-up segtree, supports only point updates