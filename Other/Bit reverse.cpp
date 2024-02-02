//Reverses bits of n
//mode == -1 -> reverses all bits of T (uint8_t x = 1 => f(x) = 128)
//mode == 0 -> reverses actual bits of n (f(6) = 3)
//O(log(bits)) = O(log(log(n)))
template<typename T>
T bit_reverse(T n, const int mode = 0) {
    static_assert(is_integral_v<T> && !is_signed_v<T>);
    int bits = sizeof(T) * 8; 
    for (T mask = ~T(0); bits >>= 1;) {
        mask ^= mask << bits;
        n = ((n & ~mask) >> bits) | ((n & mask) << bits);
    }
    n >>= mode == -1 ? 0 : __builtin_ctzll(n);
    return n;
}
