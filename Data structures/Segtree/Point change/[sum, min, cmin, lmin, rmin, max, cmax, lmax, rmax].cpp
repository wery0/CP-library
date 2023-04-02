template<typename T>
class segtree_point_upd {

    size_t n, U;
    vector<T> sm;
    vector<T> mn, cmn, lmn, rmn;
    vector<T> mx, cmx, lmx, rmx;

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];

        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        cmn[v] = (mn[v << 1] == mn[v] ? cmn[v << 1] : 0) + (mn[v << 1 | 1] == mn[v] ? cmn[v << 1 | 1] : 0);
        lmn[v] = mn[v << 1] == mn[v] ? lmn[v << 1] : lmn[v << 1 | 1];
        rmn[v] = mn[v << 1 | 1] == mn[v] ? rmn[v << 1 | 1] : rmn[v << 1];

        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
        cmx[v] = (mx[v << 1] == mx[v] ? cmx[v << 1] : 0) + (mx[v << 1 | 1] == mx[v] ? cmx[v << 1 | 1] : 0);
        lmx[v] = mx[v << 1] == mx[v] ? lmx[v << 1] : lmx[v << 1 | 1];
        rmx[v] = mx[v << 1 | 1] == mx[v] ? rmx[v << 1 | 1] : rmx[v << 1];
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        sm.resize(U * 2);

        mn.resize(U * 2);
        cmn.resize(U * 2);
        lmn.resize(U * 2);
        rmn.resize(U * 2);

        mx.resize(U * 2);
        cmx.resize(U * 2);
        lmx.resize(U * 2);
        rmx.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            const T val = *(first + i);
            sm[U + i] = val;

            mn[U + i] = val;
            cmn[U + i] = 1;
            lmn[U + i] = i;
            rmn[U + i] = i;

            mx[U + i] = val;
            cmx[U + i] = 1;
            lmx[U + i] = i;
            rmx[U + i] = i;
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
        T ans = mx[ql];
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) ans = max(ans, mx[ql]);
            if (~qr & 1) ans = max(ans, mx[qr]);
        }
        return ans;
    }

    array<T, 4> seg_min_detailed(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mn[ql], cnt = 0, psl = ql - U, psr = ql - U;
        auto go = [&](size_t i) {
            if (mn[i] < ans) ans = mn[i], cnt = cmn[i], psl = lmn[i], psr = rmn[i];
            else if (mn[i] == ans) cnt += cmn[i], psl = min(psl, lmn[i]), psr = max(psr, rmn[i]);
        };
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) go(ql);
            if (~qr & 1) go(qr);
        }
        return {ans, cnt, psl, psr};
    }

    array<T, 4> seg_max_detailed(size_t ql, size_t qr) const {
        ql += U, qr += U;
        T ans = mx[ql], cnt = 0, psl = ql - U, psr = ql - U;
        auto go = [&](size_t i) {
            if (mx[i] > ans) ans = mx[i], cnt = cmx[i], psl = lmx[i], psr = rmx[i];
            else if (mx[i] == ans) cnt += cmx[i], psl = min(psl, lmx[i]), psr = max(psr, rmx[i]);
        };
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) go(ql);
            if (~qr & 1) go(qr);
        }
        return {ans, cnt, psl, psr};
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