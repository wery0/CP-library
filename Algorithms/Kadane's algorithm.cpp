//shortest subsegment with max sum
template<typename I>
auto subsegment_with_max_sum(I first, I last) {
    ll cur = 0, mxs = 0;
    pii seg_borders = { -1, -1};
    int cur_l = -1;
    auto cit = first;
    for (int q = 0; cit != last; ++q, ++cit) {
        cur += *cit;
        if (cur > mxs || (cur == mxs && q - cur_l < seg_borders.S - seg_borders.F)) {
            mxs = cur;
            seg_borders = {cur_l + 1, q};
        }
        if (cur <= 0) {
            cur = 0;
            cur_l = q;
        }
    }
    if (seg_borders.F == -1) return *max_element(first, last);
    return mxs;
}

//shortest subsegment with min sum
template<typename I>
auto subsegment_with_min_sum(I first, I last) {
    ll cur = 0, mns = 0;
    pii seg_borders = { -1, -1};
    int cur_l = -1;
    auto cit = first;
    for (int q = 0; cit != last; ++q, ++cit) {
        cur += *cit;
        if (cur < mns || (cur == mns && q - cur_l < seg_borders.S - seg_borders.F)) {
            mns = cur;
            seg_borders = {cur_l + 1, q};
        }
        if (cur >= 0) {
            cur = 0;
            cur_l = q;
        }
    }
    if (seg_borders.F == -1) return *min_element(first, last);
    return mns;
}