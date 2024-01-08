template<typename T>
class segtree_point_upd {

    size_t n;
    vector<T> orr;
    vector<T> andd;
    vector<T> xorr;

    void upd(size_t v) {
        orr[v] = orr[v << 1] | orr[v << 1 | 1];
        andd[v] = andd[v << 1] & andd[v << 1 | 1];
        xorr[v] = xorr[v << 1] ^ xorr[v << 1 | 1];
    }

    void init_node_by_value(size_t pos, const T& val) {
        orr[pos] = andd[pos] = xorr[pos] = val;
    }

public:
    segtree_point_upd() = default;

    template<typename I>
    segtree_point_upd(I first, I last): n(std::distance(first, last)) {
        if (!n) return;
        n = n & (n - 1) ? 2 << __lg(n) : n;
        orr.resize(n * 2);
        andd.resize(n * 2);
        xorr.resize(n * 2);
        for (size_t i = 0; first != last; ++i, ++first) {
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

    T seg_or(size_t l, size_t r) const {
        l += n, r += n;
        T ans = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans |= orr[l];
            if (~r & 1) ans |= orr[r];
        }
        return ans;
    }

    T seg_and(size_t l, size_t r) const {
        l += n, r += n;
        T ans = andd[l];
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans &= andd[l];
            if (~r & 1) ans &= andd[r];
        }
        return ans;
    }

    T seg_xor(size_t l, size_t r) const {
        l += n, r += n;
        T ans = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) ans ^= xorr[l];
            if (~r & 1) ans ^= xorr[r];
        }
        return ans;
    }

    int first_to_right_where_or_has_geq_k_bits(size_t l, T k) {
        size_t r = n * 2 - 1;
        T u = 0;
        for (l += n; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (~l & 1) continue;
            if (__builtin_popcountll(u | orr[l]) < k) u |= orr[l];
            else {
                while (l < n) {
                    l <<= 1;
                    if (__builtin_popcountll(u | orr[l]) < k) u |= orr[l], l ^= 1;
                }
                return l - n;
            }
        }
        return -1;
    }

    void point_change(size_t pos, T val) {
        pos += n;
        init_node_by_value(pos, val);
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }

    T operator[](size_t pos) const {
        return orr[n + pos];
    }
};
