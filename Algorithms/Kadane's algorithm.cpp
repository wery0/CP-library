//Finds shortest subsegment with max sum
//Sum of any subsegment must fit in T
template<typename I, typename T = long long>
T subsegment_with_max_sum(I first, I last) {
    T cur = 0, mxs = 0;
    array<int, 2> seg_borders = {-1, -1};
    int cur_l = -1;
    auto cit = first;
    for (int i = 0; cit != last; ++i, ++cit) {
        cur += *cit;
        if (cur > mxs || (cur == mxs && i - cur_l < seg_borders[1] - seg_borders[0])) {
            mxs = cur;
            seg_borders = {cur_l + 1, i};
        }
        if (cur <= 0) cur = 0, cur_l = i;
    }
    if (seg_borders[0] == -1) return *max_element(first, last);
    return mxs;
}

//Finds shortest subsegment with min sum
//Sum of any subsegment must fit in T
template<typename I, typename T = long long>
T subsegment_with_min_sum(I first, I last) {
    T cur = 0, mns = 0;
    array<int, 2> seg_borders = {-1, -1};
    int cur_l = -1;
    auto cit = first;
    for (int i = 0; cit != last; ++i, ++cit) {
        cur += *cit;
        if (cur < mns || (cur == mns && i - cur_l < seg_borders[1] - seg_borders[0])) {
            mns = cur;
            seg_borders = {cur_l + 1, i};
        }
        if (cur >= 0) cur = 0, cur_l = i;
    }
    if (seg_borders[0] == -1) return *min_element(first, last);
    return mns;
}