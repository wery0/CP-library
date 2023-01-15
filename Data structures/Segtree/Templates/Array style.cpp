template<typename T>
class segtree {

    const uint a, U;
    //Create needed vectors

    inline int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void push(uint v) {
        //Write push, if need

    }

    void upd(uint v) {
        //Write update

    }

    T seg_statistic(uint ql, uint qr, uint l, uint r, uint v) {
        if (qr < l || r < ql) return neutral_element;
        if (ql <= l && r <= qr) {
            //Return needed statistic

        }
        push(v);
        uint md = l + (r - l) >> 1;
        const auto lf = seg_statistic(ql, qr, l, md, v << 1);
        const auto rg = seg_statistic(ql, qr, md + 1, r, v << 1 | 1);
        return ?;
    }

    void seg_change(uint ql, uint qr, uint l, uint r, uint v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //Write change function

            return;
        }
        push(v);
        uint md = l + (r - l) >> 1;
        seg_change(ql, qr, l, md, v << 1, val);
        seg_change(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

public:
    segtree() = default;
    template<typename I>
    segtree(I first, I last): a(last - first), U(geq_pow2(a)) {
        //Resize needed vectors with U * 2

        for (uint q = 0; q < a; q++) {
            const T val = *(first + q);
            //Init last layer. Indices of last layer is U + q.
        }
        for (uint q = U; --q;) {
            upd(q);
            //Write additional info, if need
        }
    }
    template<typename T_arr>
    segtree(T_arr& n) {
        (*this) = segtree<T>(n.begin(), n.end());
    }
    segtree(uint a) {
        vector<T> m(a);
        (*this) = segtree<T>(m);
    }

    T seg_statistic(uint ql, uint qr) {return seg_statistic(ql, qr, 0, U - 1, 1);}
    inline void seg_change(uint ql, uint qr, T val) {seg_change(ql, qr, 0, U - 1, 1, val);}

    // T operator[](uint pos) {
    //     uint l = 0, r = U - 1, v = 1;
    //     T o = 0;
    //     while (l != r) {
    //         push(v);
    //         uint md = l + (r - l) >> 1;
    //         if (pos <= md) {
    //             r = md;
    //             v = v << 1;
    //         } else {
    //             l = md + 1;
    //             v = v << 1 | 1;
    //         }
    //     }
    //     return sm[v];
    // }

    // void get_last_layer(vector<T>& res) {
    //     for (int q = 1; q < U; q++) push(q);
    //     for (int q = 0; q < res.size(); ++q) {
    //         res[q] = sm[U + q];
    //     }
    // }
};