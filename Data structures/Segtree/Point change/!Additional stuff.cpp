//You may add some of these functions inside segtree_point_upd.
//These functions are working only when the size of last layer of segtree is perfect power of 2. To do so, in constructor add n = n & (n - 1) ? 2 << __lg(n) : n;

//Works only with nonnegative values
int first_to_right_where_sum_becomes_geq_k(size_t l, T k) {
    size_t r = n * 2 - 1;
    for (l += n; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
        if (~l & 1) continue;
        if (m[l].sm < k) k -= m[l].sm;
        else {
            while (l < n) {
                l <<= 1;
                if (m[l].sm < k) k -= m[l].sm, l ^= 1;
            }
            return l - n;
        }
    }
    return -1;
}

//Works only with nonnegative values
int first_to_left_where_sum_becomes_geq_k(size_t r, T k) {
    size_t l = n;
    for (r += n; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
        if (r & 1) continue;
        if (m[r].sm < k) k -= m[r].sm;
        else {
            while (r < n) {
                r = r << 1 | 1;
                if (m[r].sm < k) k -= m[r].sm, r ^= 1;
            }
            return r - n;
        }
    }
    return -1;
}
