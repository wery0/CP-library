//Maps (x, y) -> position in sierpinski curve
//Position can be up to 4 * C ^ 2
//O(log(C))
template<typename T = uint64_t>
T sierpinski_encode(T x, T y, T C = 1 << 20) {
    assert(max(x, y) <= C);
    T p = 0;
    if (x > y) {
        ++p;
        x = C - x;
        y = C - y;
    }
    for (T it = C, px = 0; it; it >>= 1) {
        p *= 2;
        if (x + y > C) {
            ++p;
            px = x;
            x = C - y;
            y = px;
        }
        x *= 2, y *= 2, p *= 2;
        if (y > C) {
            ++p;
            px = x;
            x = y - C;
            y = C - px;
        }
    }
    return p;
}
