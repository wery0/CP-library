/*
Counts position of highest set bit of n
Idea:
  Split bits of n into blocks of sqrt(w) length.
  Calculate mask of length sqrt(w) ith bit of which == 1 <=> ith block has 1.
  To get the answer, calculate highest set bit in the mask then do the same for corresponding block.
Complexity: O(1) with big constant
*/
template<typename T>
T bit_lg(T n) {
    static_assert(is_integral_v<T> && !is_signed_v<T>);
    static constexpr int w = sizeof(T) * 8;
    static constexpr int block = sqrt(w / 2) * sqrt(w / 2) == w / 2 ? sqrt(w / 2) : sqrt(w / 4);
    static constexpr T m1 = [&]() {T m = 0; for (int i = 0; i < block; ++i) m |= T(1) << block * i; return m << (block - 1);}();
    static constexpr T m2 = [&]() {T m = 0; for (int i = 0; i < block; ++i) m |= T(1) << (block - 1) * i; return m;}();
    static constexpr T m3 = [&]() {T m = 0; for (int i = 0; i < block; ++i) m |= T(1) << (block + 1) * i; return m;}();
    static constexpr T m4 = [&]() {T m = 0; for (int i = 0; i < block; ++i) m |= T(1) << (block + 2) * i; return m;}();
    function<int(T)> lg_of_small = [&](T x) {
        x &= (T(1) << block) - 1;
        x *= m3;
        x |= m3 << block;
        x -= m4;
        x &= m3 << block;
        x *= m3;
        x >>= (block + 1) * block - 1;
        x &= (T(1) << block) - 1;
        return x - 1;
    };
    if (n == 0) return -1;
    T add = 0;
    if (n >> w / 2) n >>= w / 2, add += w / 2;
    else n &= ~T(0) >> w / 2;
    if (block * block != w / 2) {
        if (n >> w / 4) n >>= w / 4, add += w / 4;
        else n &= ~T(0) >> w / 4 * 3;
    }
    T stn = n;
    n = (n | m1) - (m1 >> block - 1);
    int p1 = lg_of_small((stn & m1 | n & m1) * m2 >> block * (block - 1));
    int p2 = lg_of_small(stn >> p1 * block);
    return add + p1 * block + p2;
}
