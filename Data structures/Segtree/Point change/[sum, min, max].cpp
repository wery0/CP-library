template<typename T>
class segtree_point_upd {

    size_t n;
    vector<T> sm;
    vector<T> mn;
    vector<T> mx;

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

    void init_node_by_value(size_t pos, const T& val) {
        sm[pos] = mn[pos] = mx[pos] = val;
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(last - first) {
        if (!n) return;
        sm.resize(n * 2);
        mn.resize(n * 2);
        mx.resize(n * 2);
        for (size_t i = 0; i < n; ++i) {
            init_node_by_value(n + i, *(first + i));
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
        T ans = mx[r];
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans = max(ans, mx[l]);
            if (~r & 1) ans = max(ans, mx[r]);
        }
        return ans;
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
