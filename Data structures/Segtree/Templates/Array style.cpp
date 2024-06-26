template<typename T>
class segtree {

    size_t n, U;
    //Create needed vectors

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void push(size_t v) {
        //Write push, if need

    }

    void upd(size_t v) {
        //Write update

    }

    T seg_statistic(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return neutral_element;
        if (ql <= l && r <= qr) {
            //Return needed statistic

        }
        push(v);
        size_t md = (l + r) >> 1;
        const auto lf = seg_statistic(ql, qr, l, md, v << 1);
        const auto rg = seg_statistic(ql, qr, md + 1, r, v << 1 | 1);
        return ?;
    }

    void seg_change(size_t ql, size_t qr, size_t l, size_t r, size_t v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //Write change function

            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_change(ql, qr, l, md, v << 1, val);
        seg_change(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(std::distance(first, last)), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        //Resize needed vectors with U * 2

        for (size_t i = 0; first != last; ++i, ++first) {
            const T val = *first;
            //Init last layer. Indices of last layer is U + i.
        }
        for (size_t i = U; --i;) {
            upd(i);
            //Write additional info, if need
        }
    }

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree<T>(m.begin(), m.end());
        } else {
            (*this) = segtree<T>(n.begin(), n.end());
        }
    }

    T seg_statistic(size_t l, size_t r) {return seg_statistic(l, r, 0, U - 1, 1);}
    void seg_change(size_t l, size_t r, T val) {seg_change(l, r, 0, U - 1, 1, val);}

    // T operator[](size_t pos) {
    //     size_t l = 0, r = U - 1, v = 1;
    //     T o = 0;
    //     while (l != r) {
    //         push(v);
    //         size_t md = l + (r - l) >> 1;
    //         if (pos <= md) r = md, v = v << 1;
    //         else l = md + 1, v = v << 1 | 1;
    //     }
    //     return sm[v];
    // }

    // vector<T> get_last_layer() {
    //     for (size_t i = 1; i < U; ++i) push(i);
    //     return vector<T>(sm.begin() + U, sm.begin() + U + n);
    // }
};
