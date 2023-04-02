template<typename T>
class segtree_point_upd {

    size_t n, U;
    vector<T> sm;
    vector<T> mn;
    vector<T> mx;

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);
        mn.resize(U * 2);
        mx.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            sm[U + i] = val;
            mn[U + i] = val;
            mx[U + i] = val;
        }
        for (size_t i = U; --i;) upd(i);
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

    T seg_sum(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans += sm[ql];
            if (~qr & 1) ans += sm[qr];
        }
        return ans;
    }

    T seg_min(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mn[ql];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans = min(ans, mn[ql]);
            if (~qr & 1) ans = min(ans, mn[qr]);
        }
        return ans;
    }

    T seg_max(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mx[qr];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans = max(ans, mx[ql]);
            if (~qr & 1) ans = max(ans, mx[qr]);
        }
        return ans;
    }

    void point_change(size_t pos, T val) {
        pos += U;
        sm[pos] = mn[pos] = mx[pos] = val;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }

    T operator[](size_t pos) const {
        return sm[U + pos];
    }
};