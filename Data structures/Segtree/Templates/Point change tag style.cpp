template<typename T>
class segtree_point_upd {

    struct tag {

        tag() = default;

        friend bool is_neutral(const tag& t) {
            //Write neutral if need, or remove it
            return false;
        }

        friend void merge(const tag& l, const tag& r, tag& res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //Write merge
        }

        friend tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

//        friend void merge_to_left(tag& l, const tag& r) {
//            if (is_neutral(l)) {l = r; return;}if (is_neutral(r)) return;
//
//        }
//
//        friend void merge_to_right(const tag& l, tag& r) {
//            if (is_neutral(l)) return;if (is_neutral(r)) {r = l; return;}
//
//        }
    };

    tag neutral_tag;    //Init neutral tag

    size_t n;
    vector<tag> m;

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    //Write this
    void init_tag_by_value(tag& t, const T& val) {
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): n(std::distance(first, last)) {
        if (!n) return;
        //Make n power of 2, if need
        m.resize(n * 2);
        for (size_t i = 0; first != last; ++i, ++first) {
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

    tag seg_statistic(size_t l, size_t r) const {
        l += n, r += n;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) lt = merge(lt, m[l]);
            if (~r & 1) rt = merge(m[r], rt);
//            if (l & 1) merge_to_left(lt, m[l]);
//            if (~r & 1) merge_to_right(m[r], rt);
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
