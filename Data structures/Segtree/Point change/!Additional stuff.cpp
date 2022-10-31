//You may add some of these functions inside point_change_segtree.

//Works only with nonnegative values
inline int first_to_right_where_sum_becomes_geq_k(uint ql, T k) {
    ql += U;
    uint qr = U + U - 1;
    for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
        if (ql & 1) {
            if (m[ql].sm < k) k -= m[ql].sm;
            else {
                uint v = ql;
                for (; v < U; ) {
                    v <<= 1;
                    if (m[v].sm < k) k -= m[v].sm, v |= 1;
                }
                k -= m[v].sm;
                return v - U;
            }
        }
    }
    return -1;
}

//Works only with nonnegative values
inline int first_to_left_where_sum_becomes_geq_k(uint qr, T k) {
    uint ql = U;
    qr += U;
    for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
        if (~qr & 1) {
            if (m[qr].sm < k) k -= m[qr].sm;
            else {
                uint v = qr;
                for (; v < U; ) {
                    v <<= 1;
                    if (m[v | 1].sm < k) k -= m[v | 1].sm;
                    else v |= 1;
                }
                k -= m[v].sm;
                return v - U;
            }
        }
    }
    return -1;
}