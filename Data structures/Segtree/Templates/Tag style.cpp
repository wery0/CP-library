template<typename T>
class segtree {

    struct tag {

        tag() = default;

        friend void push_tag(tag& t, tag& l, tag& r) {
            //Write push if need, or remove it
        }

        friend bool is_neutral(const tag& t) {
            //Write is_neutral if need, or remove it
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
    };

    tag neutral_tag;   //Init neutral tag

    size_t n, U;
    vector<tag> m;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void push(size_t v) {
        push_tag(m[v], m[v << 1], m[v << 1 | 1]);
    }

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    tag seg_statistic(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return neutral_tag;
        if (ql <= l && r <= qr) {
            return m[v];
        }
        push(v);
        size_t md = (l + r) >> 1;
        const tag lf = seg_statistic(ql, qr, l, md, v << 1);
        const tag rg = seg_statistic(ql, qr, md + 1, r, v << 1 | 1);
        return merge(lf, rg);
    }

    void seg_change(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //Write change function
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_change(ql, qr, l, md, v << 1);
        seg_change(ql, qr, md + 1, r, v << 1 | 1);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
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

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree<T>(m.begin(), m.end());
        } else {
            (*this) = segtree<T>(n.begin(), n.end());
        }
    }

    tag seg_statistic(size_t l, size_t r) {return seg_statistic(l, r, 0, U - 1, 1);}
    void seg_change(size_t l, size_t r) {seg_change(l, r, 0, U - 1, 1);}
};