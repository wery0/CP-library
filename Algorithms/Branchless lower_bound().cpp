//https://probablydance.com/2023/04/27/beautiful-branchless-binary-search/
template<typename It, typename T, typename Cmp>
It branchless_lower_bound(It begin, It end, const T& value, Cmp&& compare) {
    size_t length = end - begin;
    if (length == 0) return end;
    size_t step = size_t(1) << (sizeof(length) * 8 - countl_zero(length) - 1);
    if (step != length && compare(begin[step], value)) {
        length -= step + 1;
        if (length == 0) return end;
        step = size_t(1) << (sizeof(length) * 8 - countl_zero(length - 1));
        begin = end - step;
    }
    for (step /= 2; step != 0; step /= 2) {
        if (compare(begin[step], value)) begin += step;
    }
    return begin + compare(*begin, value);
}
template<typename It, typename T>
It branchless_lower_bound(It begin, It end, const T& value) {
    return branchless_lower_bound(begin, end, value, std::less<>{});
}
template<typename It, typename T>
It branchless_upper_bound(It begin, It end, const T& value) {
    return branchless_lower_bound(begin, end, value, std::less_equal<>{});
}
