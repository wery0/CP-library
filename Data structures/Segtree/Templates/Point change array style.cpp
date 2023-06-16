template<typename T>
class segtree_point_upd {

    size_t n;
    //Create needed vectors

    void upd(size_t v) {
        //Write update
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(last - first) {
        if (!n) return;
        //Resize needed vectors with n * 2
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            //Write init of last layer. Indices of last layer is n + i.
        }
        for (size_t i = n; --i;) {
            upd(i);
            //Write additional info, if need
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

    T seg_statistic(size_t l, size_t r) const {
        l += n, r += n;
        T ans = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            //Change merge
            if (l & 1) merge(ans, arr[l]);
            if (~r & 1) merge(arr[r], ans);
        }
        return ans;
    }

    void point_change(size_t pos, T val) {
        pos += n;
        //Change element in needed vectors
        //sm[pos] = mn[pos] = mx[pos] = val;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//Bottom-up segtree, supports only point updates