template<typename T>
class segtree_point_upd {

    size_t a, U;
    vector<T> sm;
    vector<T> mn;
    vector<T> mx;

    inline void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): a(last - first), U(geq_pow2(a)) {
        sm.resize(U * 2);
        mn.resize(U * 2);
        mx.resize(U * 2);
        for (size_t q = 0; q < a; ++q) {
            const T val = *(first + q);
            sm[U + q] = val;
            mn[U + q] = val;
            mx[U + q] = val;
        }
        for (size_t q = U; --q;) {
            upd(q);
        }
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral<U>::value) {
            vector<T> m(n);
            (*this) = segtree_point_upd<T>(m.begin(), m.end());
        } else {
            (*this) = segtree_point_upd<T>(n.begin(), n.end());
        }
    }

    inline T seg_sum(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans += sm[ql];
            if (~qr & 1) ans += sm[qr];
        }
        return ans;
    }

    inline T seg_min(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mn[ql];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans = min(ans, mn[ql]);
            if (~qr & 1) ans = min(ans, mn[qr]);
        }
        return ans;
    }

    inline T seg_max(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mx[qr];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans = max(ans, mx[ql]);
            if (~qr & 1) ans = max(ans, mx[qr]);
        }
        return ans;
    }

    inline void point_change(size_t p, T val) {
        p += U;
        sm[p] = mn[p] = mx[p] = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }

    T operator[](size_t pos) const {
        return sm[U + pos];
    }
};
//Supports only point updates