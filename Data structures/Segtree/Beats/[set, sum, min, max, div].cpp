template<typename T>
class segtree {

    static constexpr T INF = numeric_limits<T>::max();

    size_t n, U;
    vector<T> sm, mn, mx;
    vector<T> ps_set;
    vector<char> is_set;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply_set(int v, T val) {
        sm[v] = val * gsz(v);
        mn[v] = val;
        mx[v] = val;
        ps_set[v] = val;
        is_set[v] = 1;
    }

    void push(size_t v) {
        if (is_set[v]) {
            apply_set(v << 1, ps_set[v]);
            apply_set(v << 1 | 1, ps_set[v]);
            is_set[v] = 0;
        }
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

    T seg_max(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return -INF;
        if (ql <= l && r <= qr) return mx[v];
        push(v);
        size_t md = (l + r) >> 1;
        return max(seg_max(ql, qr, l, md, v << 1),
                   seg_max(ql, qr, md + 1, r, v << 1 | 1));
    }

    T seg_min(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return mn[v];
        push(v);
        size_t md = (l + r) >> 1;
        return min(seg_min(ql, qr, l, md, v << 1),
                   seg_min(ql, qr, md + 1, r, v << 1 | 1));
    }

    T seg_sum(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sm[v];
        push(v);
        size_t md = (l + r) >> 1;
        return seg_sum(ql, qr, l, md, v << 1) +
               seg_sum(ql, qr, md + 1, r, v << 1 | 1);
    }

    void seg_set(size_t ql, size_t qr, size_t l, size_t r, size_t v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_set(v, val);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_set(ql, qr, l, md, v << 1, val);
        seg_set(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

    void seg_div(size_t ql, size_t qr, size_t l, size_t r, size_t v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr && mn[v] == mx[v]) {
            apply_set(v, mn[v] / val);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_div(ql, qr, l, md, v << 1, val);
        seg_div(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(std::distance(first, last)), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        mn.resize(U * 2, INF);
        mx.resize(U * 2, -INF);
        ps_set.resize(U * 2, NO_PUSH_SET);
        for (size_t i = 0; i < n; ++i, ++first) {
            const T val = *first;
            sm[U + i] = val;
            mn[U + i] = val;
            mx[U + i] = val;
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
    T seg_min(size_t l, size_t r) {return seg_min(l, r, 0, U - 1, 1);}
    T seg_max(size_t l, size_t r) {return seg_max(l, r, 0, U - 1, 1);}
    void seg_set(size_t l, size_t r, T val) {seg_set(l, r, 0, U - 1, 1, val);}
    void seg_div(size_t l, size_t r, T val) {if (x == 1) return; seg_div(l, r, 0, U - 1, 1, val);}
};
