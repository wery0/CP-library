template<typename T, const size_t N>
class segtree {
    static_assert(N <= 6);  //Add more Faulhaber's formulas if you need to work with bigger polynomials

    static constexpr auto C = []() {
        array<array<T, N>, N> C;
        fill(C[0].begin(), C[0].end(), 1);
        for (size_t i = 1; i < N; ++i) {
            C[i][0] = 1;
            for (size_t j = 1; j < N - i; ++j) {
                C[i][j] = C[i - 1][j] + C[i][j - 1];
            }
        }
        return C;
    }();

    using poly = array<T, N>;
    void add_to(poly& l, const poly& r) {for (size_t i = 0; i < N; ++i) l[i] += r[i];}
    poly sum(poly l, const poly& r) {add_to(l, r); return l;}

    size_t n, U;
    vector<T> sm;
    vector<poly> ps;
    vector<char> is_set;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    poly gnc(const poly& p, T x) {
        static array<T, N> pw{1};
        for (size_t i = 1; i < N; ++i) pw[i] = pw[i - 1] * x;
        poly n{0};
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; i + j < N; ++j) {
                n[i] += C[i][j] * p[i + j] * pw[j];
            }
        }
        return n;
    }

    //Faulhaber's formulas
    T eval(const poly& p, T x) {
        T res = p[0];
        if constexpr(N > 0) res += x * p[0];
        if constexpr(N > 1) res += x * (x + 1) / 2 * p[1];
        if constexpr(N > 2) res += x * (x + 1) * (2 * x + 1) / 6 * p[2];
        if constexpr(N > 3) res += x * x * (x + 1) * (x + 1) / 4 * p[3];
        if constexpr(N > 4) res += x * (x + 1) * (2 * x + 1) * (3 * x * x + 3 * x - 1) / 30 * p[4];
        if constexpr(N > 5) res += x * x * (x + 1) * (x + 1) * (2 * x * x + 2 * x - 1) / 12 * p[5];
        return res;
    }

    void apply_set(int v, const poly& p) {
        ps[v] = p;
        sm[v] = eval(p, gsz(v) - 1);
        is_set[v] = 1;
    }

    void apply_add(int v, const poly& p) {
        add_to(ps[v], p);
        sm[v] += eval(p, gsz(v) - 1);
    }

    void push(size_t v) {
        if (is_set[v]) {
            apply_set(v << 1, ps[v]);
            apply_set(v << 1 | 1, gnc(ps[v], gsz(v << 1)));
            fill(ps[v].begin(), ps[v].end(), 0);
            is_set[v] = 0;
        } else if (any_of(ps[v].begin(), ps[v].end(), [](T c) {return c != 0;})) {
            apply_add(v << 1, ps[v]);
            apply_add(v << 1 | 1, gnc(ps[v], gsz(v << 1)));
            fill(ps[v].begin(), ps[v].end(), 0);
        }
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

    void seg_add_polynomial(size_t ql, size_t qr, size_t l, size_t r, size_t v, const poly& p) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, p);
            return;
        }
        push(v);
        ssize_t md = (l + r) >> 1;
        seg_add_polynomial(ql, qr, l, md, v << 1, p);
        ssize_t d = max((ssize_t)0, md - (ssize_t)max(ql, l) + 1);
        seg_add_polynomial(ql, qr, md + 1, r, v << 1 | 1, gnc(p, d));
        upd(v);
    }

    void seg_set_polynomial(size_t ql, size_t qr, size_t l, size_t r, size_t v, const poly& p) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_set(v, p);
            return;
        }
        push(v);
        ssize_t md = (l + r) >> 1;
        seg_set_polynomial(ql, qr, l, md, v << 1, p);
        ssize_t d = max((ssize_t)0, md - (ssize_t)max(ql, l) + 1);
        seg_set_polynomial(ql, qr, md + 1, r, v << 1 | 1, gnc(p, d));
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(std::distance(first, last)), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        ps.resize(U * 2);
        is_set.resize(U * 2);
        copy(first, last, sm.begin() + U);
        for (size_t i = U; --i;) upd(i);
    }

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree<T, N>(m.begin(), m.end());
        } else {
            (*this) = segtree<T, N>(n.begin(), n.end());
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
        vector<T> ans(sm.begin() + U, sm.begin() + U + n);
        return ans;
    }

    T seg_sum(size_t l, size_t r) {return seg_sum(l, r, 0, U - 1, 1);}

    //a[l + i] +=(=) sum{p[j] * pow(i, j) | 0 <= j < p.size()}, 0 <= i <= r - l
    void seg_add_polynomial(size_t l, size_t r, const poly& p) {seg_add_polynomial(l, r, 0, U - 1, 1, p);}
    void seg_set_polynomial(size_t l, size_t r, const poly& p) {seg_set_polynomial(l, r, 0, U - 1, 1, p);}
};
