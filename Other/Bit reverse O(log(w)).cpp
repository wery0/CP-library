/*
Reverses bits of n
  mode == -1 -> reverses all bits of T (uint8_t x = 1 => f(x) = 128)
  mode == 0 -> reverses actual bits of n (f(6) = 3)
Complexity: O(log(w)) = O(log(log(C))) where C is max value of T
*/
template<typename T>
T bit_reverse(T n, const int mode = 0) {
    static_assert(is_integral_v<T> && !is_signed_v<T>);
    static constexpr int w = sizeof(T) * 8;
    for (T mask = ~T(0), b = w; b >>= 1;) {
        mask ^= mask << b;
        n = ((n & ~mask) >> b) | ((n & mask) << b);
    }
    n >>= mode == -1 ? 0 : __builtin_ctzll(n);
    return n;
}
