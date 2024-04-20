template<typename T>
class segtree_point_upd {

    size_t n;
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

    void init_node_by_value(size_t pos, const T& val) {
        sm[pos] = mn[pos] = mx[pos] = val;
        cmn[pos] = cmx[pos] = 1;
        lmn[pos] = rmn[pos] = lmx[pos] = rmx[pos] = i;
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(std::distance(first, last)) {
        if (!n) return;
        sm.resize(n * 2);

        mn.resize(n * 2);
        cmn.resize(n * 2);
        lmn.resize(n * 2);
        rmn.resize(n * 2);

        mx.resize(n * 2);
        cmx.resize(n * 2);
        lmx.resize(n * 2);
        rmx.resize(n * 2);
        for (size_t i = 0; i < n; ++i, ++first) {
            init_node_by_value(n + i, *first);
        }
        for (size_t i = n; --i;) upd(i);
    }

    template<typename U>
    segtree_point_upd(U n) {
        if constexpr(is_integral_v<U>) {
            vector<T> m(n);
            (*this) = segtree_point_upd<T>(m.begin(), m.end());
        } else {
            (*this) = segtree_point_upd<T>(n.begin(), n.end());
        }
    }

    T seg_sum(size_t l, size_t r) const {
        l += n, r += n;
        T ans = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans += sm[l];
            if (~r & 1) ans += sm[r];
        }
        return ans;
    }

    T seg_min(size_t l, size_t r) const {
        l += n, r += n;
        T ans = mn[l];
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans = min(ans, mn[l]);
            if (~r & 1) ans = min(ans, mn[r]);
        }
        return ans;
    }

    T seg_max(size_t l, size_t r) const {
        l += n, r += n;
        T ans = mx[l];
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans = max(ans, mx[l]);
            if (~r & 1) ans = max(ans, mx[r]);
        }
        return ans;
    }

    array<T, 4> seg_min_detailed(size_t l, size_t r) const {
        l += n, r += n;
        T ans = mn[l], cnt = 0, psl = l - n, psr = l - n;
        auto go = [&](size_t i) {
            if (mn[i] < ans) ans = mn[i], cnt = cmn[i], psl = lmn[i], psr = rmn[i];
            else if (mn[i] == ans) cnt += cmn[i], psl = min(psl, lmn[i]), psr = max(psr, rmn[i]);
        };
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) go(l);
            if (~r & 1) go(r);
        }
        return {ans, cnt, psl, psr};
    }

    array<T, 4> seg_max_detailed(size_t l, size_t r) const {
        l += n, r += n;
        T ans = mx[l], cnt = 0, psl = l - n, psr = l - n;
        auto go = [&](size_t i) {
            if (mx[i] > ans) ans = mx[i], cnt = cmx[i], psl = lmx[i], psr = rmx[i];
            else if (mx[i] == ans) cnt += cmx[i], psl = min(psl, lmx[i]), psr = max(psr, rmx[i]);
        };
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) go(l);
            if (~r & 1) go(r);
        }
        return {ans, cnt, psl, psr};
    }

    void point_change(size_t pos, T val) {
        pos += n;
        init_node_by_value(pos, val);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }

    T operator[](size_t pos) const {
        return sm[n + pos];
    }
};
