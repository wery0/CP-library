template<typename T>
class segtree_point_upd {

    size_t n, U;
    //Create needed vectors

    void upd(size_t v) {
        //Write update
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        //Resize needed vectors with U * 2
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            //Write init of last layer. Indices of last layer is U + i.
        }
        for (size_t i = U; --i;) {
            upd(i);
            //Write additional info, if need
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

    T seg_statistic(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            //Change merge
            if (ql & 1) merge(ans, arr[ql]);
            if (~qr & 1) merge(arr[qr], ans);
        }
        return ans;
    }

    void point_change(size_t pos, T val) {
        pos += U;
        //Change element in needed vectors
        //sm[pos] = mn[pos] = mx[pos] = val;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//Bottom-up segtree, supports only point updates