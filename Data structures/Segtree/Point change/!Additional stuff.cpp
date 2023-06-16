//You may add some of these functions inside segtree_point_upd.

//Works only with nonnegative values
int first_to_right_where_sum_becomes_geq_k(size_t l, T k) {
    size_t r = n * 2 - 1;
    for (l += n; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
        if (l & 1) {
            if (m[l].sm < k) k -= m[l].sm;
            else {
                size_t v = l;
                while (v < n) {
                    v <<= 1;
                    if (m[v].sm < k) k -= m[v].sm, v |= 1;
                }
                k -= m[v].sm;
                return v - n;
            }
        }
    }
    return -1;
}

//Works only with nonnegative values
int first_to_left_where_sum_becomes_geq_k(size_t r, T k) {
    size_t l = n;
    for (r += n; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
        if (~r & 1) {
            if (m[r].sm < k) k -= m[r].sm;
            else {
                size_t v = r;
                while (v < n) {
                    v <<= 1;
                    if (m[v | 1].sm < k) k -= m[v | 1].sm;
                    else v |= 1;
                }
                k -= m[v].sm;
                return v - n;
            }
        }
    }
    return -1;
}