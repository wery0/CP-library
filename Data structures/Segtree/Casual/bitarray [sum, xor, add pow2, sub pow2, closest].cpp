template<typename T>
class segtree {

    size_t n, U;
    vector<int> sm;
    vector<int> ps_xor;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply_xor(size_t v) {
        ps_xor[v] ^= 1;
        sm[v] = gsz(v) - sm[v];
    }

    void push(size_t v) {
        if (ps_xor[v]) {
            apply_xor(v << 1);
            apply_xor(v << 1 | 1);
            ps_xor[v] = 0;
        }
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
    }

    bool has0(int v) {return sm[v] < gsz(v);}
    bool has1(int v) {return sm[v];}
    int leftest0(int v) {assert(has0(v)); while(v < U) {push(v); v <<= 1; v ^= sm[v] == gsz(v);} return v - U;}
    int rightest0(int v) {assert(has0(v)); while(v < U) {push(v); v <<= 1; v ^= sm[v ^ 1] < gsz(v ^ 1);} return v - U;}
    int leftest1(int v) {assert(has1(v)); while(v < U) {push(v); v <<= 1; v ^= !sm[v];} return v - U;}
    int rightest1(int v) {assert(has1(v)); while(v < U) {push(v); v <<= 1; v ^= sm[v | 1] > 0;} return v - U;}

    T seg_sum(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sm[v];
        push(v);
        size_t md = (l + r) >> 1;
        const auto lf = seg_sum(ql, qr, l, md, v << 1);
        const auto rg = seg_sum(ql, qr, md + 1, r, v << 1 | 1);
        return lf + rg;
    }

    void seg_xor(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_xor(v);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_xor(ql, qr, l, md, v << 1);
        seg_xor(ql, qr, md + 1, r, v << 1 | 1);
        upd(v);
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        ps_xor.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            sm[U + i] = val;
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

    //Returns -1, if no needed number
    int closest0_from_right(size_t p) {size_t l = 0, r = U - 1, v = 1; int ans = -1; while(l < r) {push(v); size_t md = (l + r) >> 1; v <<= 1; if (p <= md) {if (has0(v | 1)) ans = v | 1; r = md;} else {v |= 1; l = md + 1;}} if (!sm[v]) ans = v; return ans == -1 ? -1 : leftest0(ans);}
    int closest0_from_left(size_t p) {size_t l = 0, r = U - 1, v = 1; int ans = -1; while(l < r) {push(v); size_t md = (l + r) >> 1; v <<= 1; if (p <= md) {r = md;} else {if (has0(v)) ans = v; v |= 1; l = md + 1;}} if (!sm[v]) ans = v; return ans == -1 ? -1 : rightest0(ans);}
    int closest1_from_left(size_t p) {size_t l = 0, r = U - 1, v = 1; int ans = -1; while(l < r) {push(v); size_t md = (l + r) >> 1; v <<= 1; if (p <= md) {r = md;} else {if (has1(v)) ans = v; v |= 1; l = md + 1;}} if (!sm[v]) ans = v; return ans == -1 ? -1 : rightest1(ans);}
    int closest1_from_right(size_t p) {size_t l = 0, r = U - 1, v = 1; int ans = -1; while(l < r) {push(v); size_t md = (l + r) >> 1; v <<= 1; if (p <= md) {if (has1(v | 1)) ans = v | 1; r = md;} else {v |= 1; l = md + 1;}} if (sm[v]) ans = v; return ans == -1 ? -1 : leftest1(ans);}

    T operator[](size_t pos) {size_t l = 0, r = U - 1, v = 1; while (l != r) {push(v); size_t md = (l + r) >> 1; if (pos <= md) {r = md; v = v << 1;} else {l = md + 1; v = v << 1 | 1;}} return sm[v];}
    vector<T> get_last_layer() {for (size_t i = 1; i < U; ++i) push(i); return vector<T>(sm.begin() + U, sm.begin() + U + n);}

    void add_pow2(int p) {int ps0 = closest0_from_right(p); assert(ps0 != -1 && "Resulting number is too big"); seg_xor(p, ps0);}
    void subtract_pow2(int p) {int ps1 = closest1_from_right(p); assert(ps1 != -1 && "Resulting number is negative"); seg_xor(p, ps1);}    
    void seg_xor(size_t l, size_t r) {seg_xor(l, r, 0, U - 1, 1);}
    T seg_sum(size_t l, size_t r) {return seg_sum(l, r, 0, U - 1, 1);}
};