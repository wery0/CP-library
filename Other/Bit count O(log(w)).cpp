/*
Counts number of set bits of n
Complexity: O(log(w)) = O(log(log(C))) where C is max value of T
*/
template<typename T>
T bit_count(T n) {
    static_assert(is_integral_v<T> && !is_signed_v<T>);
    static constexpr int w = sizeof(T) * 8;
    for (int i = 0, b = 1; b < w; ++i, b <<= 1) {
        T mask = ~T(0) / ((T(1) << b) + 1);
        n = (n & mask) + (n >> b & mask);
    }
    return n;
}

//Alternative implementation with recursion instead of loop
template<typename T, int b = 1>
constexpr T bit_count(T n) {
    static_assert(is_integral_v<T> && !is_signed_v<T>);
    if constexpr (b >= sizeof(T) * 8) {
        return n;
    } else {
        constexpr T mask = ~T(0) / ((T(1) << b) + 1);
        n = (n & mask) + ((n >> b) & mask);
        return bit_count<T, b << 1>(n);
    }
}

