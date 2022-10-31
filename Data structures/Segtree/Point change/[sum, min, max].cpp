template<typename T>
struct segtree_point_upd {

    uint a, U;
    vec<T> sm;
    vec<T> mn;
    vec<T> mx;

    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last) {
        a = last - first;
        U = geq_pow2(a);
        sm.resize(U * 2);
        mn.resize(U * 2);
        mx.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            const T val = *(first + q);
            sm[U + q] = val;
            mn[U + q] = val;
            mx[U + q] = val;
        }
        for (uint q = U; --q;) {
            upd(q);
        }
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral<U>::value) {
            vec<T> m(n);
            (*this) = segtree_point_upd<T>(all(m));
        } else {
            (*this) = segtree_point_upd<T>(all(n));
        }
    }

    inline void upd(uint v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

    inline T seg_sum(uint ql, uint qr) const {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans += sm[ql];
            if (~qr & 1) ans += sm[qr];
        }
        return ans;
    }

    inline T seg_min(uint ql, uint qr) const {
        ql += U, qr += U;
        T ans = mn[ql];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) chmin(ans, mn[ql]);
            if (~qr & 1) chmin(ans, mn[qr]);
        }
        return ans;
    }

    inline T seg_max(uint ql, uint qr) const {
        ql += U, qr += U;
        T ans = mx[qr];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) chmax(ans, mx[ql]);
            if (~qr & 1) chmax(ans, mx[qr]);
        }
        return ans;
    }

    inline void point_change(uint p, T val) {
        p += U;
        sm[p] = mn[p] = mx[p] = val;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }

    T operator[](uint pos) const {
        return sm[U + pos];
    }
};
//Supports only point updates