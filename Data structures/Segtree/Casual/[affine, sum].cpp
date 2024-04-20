template<typename T>
class segtree {

    size_t n, U;
    vector<T> sm;
    vector<T> psa;
    vector<T> psb;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply(int v, T a, T b) {
        psa[v] *= a;
        psb[v] = psb[v] * a + b;
        sm[v] = sm[v] * a +  gsz(v) * b;
    }

    void push(size_t v) {
        if (psa[v] == 1 && psb[v] == 0) return;
        apply(v << 1, psa[v], psb[v]);
        apply(v << 1 | 1, psa[v], psb[v]);
        psa[v] = 1;
        psb[v] = 0;
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
    }

    T seg_sum(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sm[v];
        push(v);
        size_t md = (l + r) >> 1;
        const auto lf = seg_sum(ql, qr, l, md, v << 1);
        const auto rg = seg_sum(ql, qr, md + 1, r, v << 1 | 1);
        return lf + rg;
    }

    void seg_affine(size_t ql, size_t qr, size_t l, size_t r, size_t v, T va, T vb) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(v, va, vb);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_affine(ql, qr, l, md, v << 1, va, vb);
        seg_affine(ql, qr, md + 1, r, v << 1 | 1, va, vb);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(std::distance(first, last)), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        psa.resize(U * 2, 1);
        psb.resize(U * 2);
        copy(first, last, sm.begin() + U);
        for (size_t i = U; --i;) upd(i);
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

    T operator[](size_t pos) {
        size_t l = 0, r = U - 1, v = 1;
        while (l != r) {
            push(v);
            size_t md = (l + r) >> 1;
            if (pos <= md) r = md, v = v << 1;
            else l = md + 1, v = v << 1 | 1;
        }
        return sm[v];
    }

    vector<T> get_last_layer() {
        for (size_t i = 1; i < U; ++i) push(i);
        return vector<T>(sm.begin() + U, sm.begin() + U + n);
    }

    T seg_sum(size_t l, size_t r) {return seg_sum(l, r, 0, U - 1, 1);}
    void seg_affine(size_t l, size_t r, T va, T vb) {seg_affine(l, r, 0, U - 1, 1, va, vb);}
};
