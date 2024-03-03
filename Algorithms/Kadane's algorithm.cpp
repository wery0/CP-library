//Finds shortest subsegment with max sum
//Sum of any subsegment must fit in T
//If first == last - returns 0
//If [first, last) doesn't contain positive numbers - returns max number
template<typename I, typename T = long long>
T subsegment_with_max_sum(I first, I last) {
    if (first == last) return 0;
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
