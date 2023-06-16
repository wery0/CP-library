template<typename T>
class segtree {

    size_t n, U;
    vector<T> sm;
    vector<T> psA;
    vector<T> psB;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply_add(int v, T a, T b) {
        psA[v] += a;
        psB[v] += b;
        sm[v] += gsz(v) * (a + b * gsz(v));
    }

    void push(size_t v) {
        apply_add(v << 1, psA[v], psB[v]);
        apply_add(v << 1 | 1, psA[v] + psB[v] * gsz(v << 1) * 2, psB[v]);
        psA[v] = psB[v] = 0;
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

    void seg_add_progression(size_t ql, size_t qr, size_t l, size_t r, size_t v, T a0, T b) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, 2 * a0 - b, b);
            return;
        }
        push(v);
        ssize_t md = (l + r) >> 1;
        seg_add_progression(ql, qr, l, md, v << 1, a0, b);
        ssize_t llen = max((ssize_t)0, md - (ssize_t)max(ql, l) + 1);
        seg_add_progression(ql, qr, md + 1, r, v << 1 | 1, a0 + llen * b, b);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        psA.resize(U * 2);
        psB.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            sm[U + i] = val * 2;
        }
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
            if (pos <= md) {
                r = md;
                v = v << 1;
            } else {
                l = md + 1;
                v = v << 1 | 1;
            }
        }
        return sm[v] / 2;
    }

    vector<T> get_last_layer() {
        for (size_t i = 1; i < U; ++i) push(i);
        vector<T> ans(sm.begin() + U, sm.begin() + U + n);
        for (T& x : ans) x /= 2;
        return ans;
    }

    T seg_sum(size_t l, size_t r) {return seg_sum(l, r, 0, U - 1, 1) / 2;}
    void seg_add_progression(size_t l, size_t r, T a0, T b) {seg_add_progression(l, r, 0, U - 1, 1, a0, b);}
};