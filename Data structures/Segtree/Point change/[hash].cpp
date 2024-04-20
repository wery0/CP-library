template<typename T, const uint64_t MOD, const uint64_t P>
class segtree_point_upd {

    size_t n;
    vector<uint64_t> pows;
    vector<uint64_t> hs;

    void upd(size_t v) {
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
    segtree_point_upd(I first, I last): n(std::distance(first, last)) {
        if (!n) return;
        pows.resize(n + 1, 1);
        for (size_t i = 1; i < n + 1; ++i) pows[i] = big_prod_mod(pows[i - 1], P);
        hs.resize(n * 2);
        for (size_t i = 0; i < n; ++i, ++first) {
            hs[n + i] = big_prod_mod(*first, pows[i]);
        }
        for (size_t i = n; --i;) upd(i);
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree_point_upd<T, MOD, P>(m.begin(), m.end());
        } else {
            (*this) = segtree_point_upd<T, MOD, P>(n.begin(), n.end());
        }
    }

    uint64_t seg_hash(size_t l, size_t r) const {
        const size_t stl = l;
        l += n, r += n;
        uint64_t ans = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans += hs[l], ans -= ans < MOD ? 0 : MOD;
            if (~r & 1) ans += hs[r], ans -= ans < MOD ? 0 : MOD;
        }
        return big_prod_mod(ans, pows[n - stl]);
    }

    void point_change(size_t pos, T val) {
        pos += n;
        hs[pos] = big_prod_mod(val, pows[pos - n]);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//segtree_point_upd<T, 4000000000000000037, 666667> kek;
