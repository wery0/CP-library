template<typename T>
class segtree_point_upd {

    const uint a, U;
    //Create needed vectors

    inline void upd(uint v) {
        //Write update
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): a(last - first), U(geq_pow2(a)) {
        //Resize needed vectors with U * 2
        for (uint q = 0; q < a; ++q) {
            const T val = *(first + q);
            //Write init of last layer. Indices of last layer is U + q.
        }
        for (uint q = U; --q;) {
            upd(q);
            //Write additional info, if need
        }
    }
    template<typename T_arr>
    segtree_point_upd(T_arr& n) {
        (*this) = segtree_point_upd<T>(n.begin(), n.end());
    }
    segtree_point_upd(uint a) {
        vector<T> m(a);
        (*this) = segtree_point_upd<T>(m);
    }

    inline T seg_statistic(uint ql, uint qr) {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            //Change merge
            if (ql & 1) merge(ans, arr[ql]);
            if (~qr & 1) merge(arr[qr], ans);
        }
        return ans;
    }

    inline void point_change(uint p, T val) {
        p += U;
        //Change element in needed vectors
        //sm[p] = mn[p] = mx[p] = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//Bottom-up segtree, supports only point updates