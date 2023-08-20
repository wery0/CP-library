//Points (x1, y1), (x2, y2) should lie on line x = bZ or y = bZ
template<typename T>
T Bgrid_dst(T b, T x1, T y1, T x2, T y2) {
    static_assert(is_signed_v<T>);
    assert(b > 0 && (x1 % b == 0 || y1 % b == 0) && (x2 % b == 0 || y2 % b == 0));
    auto dst = [](T x1, T y1, T x2, T y2) {return abs(x1 - x2) + abs(y1 - y2);};
    if ((x1 == x2 && x1 % b == 0) || (y1 == y2 && y1 % b == 0)) return dst(x1, y1, x2, y2);
    if (x1 % b == 0) swap(x1, y1), swap(x2, y2);
    T lx1 = x1 - x1 % b, rx1 = lx1 + b;
    T d1 = abs(x1 - lx1) + dst(lx1, y1, x2, y2);
    T d2 = abs(x1 - rx1) + dst(rx1, y1, x2, y2);
    return min(d1, d2);
}
