template<typename T, const uint64_t MOD, const uint64_t P>
class segtree {

    static constexpr T NO_PUSH_SET = numeric_limits<T>::max() - sqrt(numeric_limits<T>::max());   //Change, if need

    size_t n, U;
    vector<uint64_t> hs;
    vector<uint64_t> pows, smpows;
    vector<T> ps_set;

    int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    void apply_push(size_t v, T val) {
        ps_set[v] = val;
        size_t lf = v << (__lg(U) - __lg(v)), rg = lf + gsz(v) - 1;
        lf -= U, rg -= U;
        hs[v] = smpows[rg];
        hs[v] += lf && smpows[lf - 1] > hs[v] ? MOD : 0;
        hs[v] -= lf ? smpows[lf - 1] : 0;
        hs[v] = big_prod_mod(hs[v], val);
    }

    void push(size_t v) {
        if (ps_set[v] != NO_PUSH_SET) {
            apply_push(v << 1, ps_set[v]);
            apply_push(v << 1 | 1, ps_set[v]);
            ps_set[v] = NO_PUSH_SET;
        }
    }

    void upd(size_t v) {
        hs[v] = hs[v << 1] + hs[v << 1 | 1];
        hs[v] -= hs[v] < MOD ? 0 : MOD;
    }

    uint64_t seg_hash(size_t ql, size_t qr, size_t l, size_t r, size_t v) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return hs[v];
        push(v);
        size_t md = (l + r) >> 1;
        const auto lf = seg_hash(ql, qr, l, md, v << 1);
        const auto rg = seg_hash(ql, qr, md + 1, r, v << 1 | 1);
        return lf + rg < MOD ? lf + rg : lf + rg - MOD;
    }

    void seg_set(size_t ql, size_t qr, size_t l, size_t r, size_t v, T val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_push(v, val);
            return;
        }
        push(v);
        size_t md = (l + r) >> 1;
        seg_set(ql, qr, l, md, v << 1, val);
        seg_set(ql, qr, md + 1, r, v << 1 | 1, val);
        upd(v);
    }

    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    uint64_t big_prod_mod(const uint64_t x, const uint64_t y) const {
        uint64_t c = (long double)x * y / MOD;
        int64_t ans = int64_t(x * y - c * MOD) % int64_t(MOD);
        return ans < 0 ? ans + MOD : ans;
    }

public:
    segtree() = default;

    template<typename I>
    segtree(I first, I last): n(std::distance(first, last)), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        pows.resize(n + 1, 1);
        smpows.resize(n + 1, 1);
        for (size_t i = 1; i < n + 1; ++i) {
            pows[i] = big_prod_mod(pows[i - 1], P);
            smpows[i] = smpows[i - 1] + pows[i];
            smpows[i] -= smpows[i] < MOD ? 0 : MOD;
        }
        hs.resize(U * 2);
        ps_set.resize(U * 2, NO_PUSH_SET);
        for (size_t i = 0; i < n; ++i, ++first) hs[U + i] = big_prod_mod(*first, pows[i]);
        for (size_t i = U; --i;) upd(i);
    }

    template<typename U>
    segtree(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree<T, MOD, P>(m.begin(), m.end());
        } else {
            (*this) = segtree<T, MOD, P>(n.begin(), n.end());
        }
    }

    uint64_t seg_hash(size_t l, size_t r) {return big_prod_mod(seg_hash(l, r, 0, U - 1, 1), pows[n - l]);}
    void seg_set(size_t l, size_t r, T val) {seg_set(l, r, 0, U - 1, 1, val);}
};
//segtree<T, 4000000000000000037, 666667> kek;
