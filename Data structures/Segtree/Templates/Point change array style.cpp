template<typename T>
struct segtree_point_upd {

    uint a, U;
    //create needed vectors

    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        //resize need vectors with U * 2
        for (uint q = 0; q < a; ++q) {
            const T val = *(first + q);
            //write init of last layer. Indices of last layer is U + q.
        }
        for (uint q = U; --q;) {
            upd(q);
            //write additional info, of need
        }
    }

    template<typename T_arr>
    segtree_point_upd(T_arr &n) {
        (*this) = segtree_point_upd<T>(all(n));
    }

    segtree_point_upd(uint a) {
        vec<T> m(a);
        (*this) = segtree_point_upd<T>(m);
    }

    inline void upd(uint v) {
        //write update
    }

    inline T seg_statistic(uint ql, uint qr) {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            //change merge
            if (ql & 1) merge(ans, arr[ql]);
            if (~qr & 1) merge(arr[qr], ans);
        }
        return ans;
    }

    inline void point_change(uint p, T val) {
        p += U;
        //change element in arrays
        //sm[p] = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//Bottom-up segtree, supports only point updates