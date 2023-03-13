template<typename T, const uint64_t MOD, const uint64_t P>
class segtree_point_upd {

    size_t n, U;
    vector<uint64_t> pows;
    vector<uint64_t> hs;

    inline void upd(size_t v) {
        hs[v] = hs[v << 1] + hs[v << 1 | 1];
        hs[v] -= hs[v] < MOD ? 0 : MOD;
    }

    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    uint64_t big_prod_mod(const uint64_t x, const uint64_t y) const {
        uint64_t c = (long double)x * y / MOD;
        int64_t ans = int64_t(x * y - c * MOD) % int64_t(MOD);
        return ans < 0 ? ans + MOD : ans;
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        pows.resize(n + 1, 1);
        for (size_t i = 1; i < n + 1; ++i) pows[i] = big_prod_mod(pows[i - 1], P);
        hs.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            hs[U + i] = big_prod_mod(val, pows[i]);
        }
        for (size_t i = U; --i;) upd(i);
    }
    template<typename T_arr>
    segtree_point_upd(T_arr& n) {
        (*this) = segtree_point_upd<T, MOD, P>(n.begin(), n.end());
    }
    segtree_point_upd(size_t n) {
        vector<T> m(n);
        (*this) = segtree_point_upd<T, MOD, P>(m);
    }

    uint64_t seg_hash(size_t ql, size_t qr) const {
        const size_t stql = ql;
        ql += U, qr += U;
        uint64_t ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans += hs[ql], ans -= ans < MOD ? 0 : MOD;
            if (~qr & 1) ans += hs[qr], ans -= ans < MOD ? 0 : MOD;
        }
        return big_prod_mod(ans, pows[n - stql]);
    }

    void point_change(size_t pos, T val) {
        pos += U;
        hs[pos] = big_prod_mod(val, pows[pos - U]);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//segtree_point_upd<T, 4000000000000000037, 666667> kek;