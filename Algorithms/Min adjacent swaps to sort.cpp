template<typename T>
int64_t min_adjacent_swaps_to_sort(const vector<T>& m) {
    return calc_inversions(m);
}
