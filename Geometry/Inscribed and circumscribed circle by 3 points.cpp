template<typename T>
array<T, 3> calc_circumscribed_circle_by_3_points(T x1, T y1, T x2, T y2, T x3, T y3, T eps = 1e-9) {
    T zn = 4 * ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1));
    if (abs(zn) < eps) return {0, 0, -1};
    T x = ((y3 - y1) * (x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1) - (y2 - y1) * (x3 * x3 + y3 * y3 - x1 * x1 - y1 * y1)) * 2 / zn;
    T y = ((x2 - x1) * (x3 * x3 + y3 * y3 - x1 * x1 - y1 * y1) - (x3 - x1) * (x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1)) * 2 / zn;
    T r = hypotl(x2 - x, y2 - y);
    return {x, y, r};
}

template<typename T>
array<T, 3> calc_inscribed_circle_by_3_points(T x1, T y1, T x2, T y2, T x3, T y3, T eps = 1e-9) {
    T a = hypotl(x2 - x3, y2 - y3);
    T b = hypotl(x3 - x1, y3 - y1);
    T c = hypotl(x1 - x2, y1 - y2);
    T s = (a + b + c) / 2;
    T area = sqrtl(s * (s - a) * (s - b) * (s - c));
    if (area < eps) return {0, 0, -1};
    T x = (a * x1 + b * x2 + c * x3) / (a + b + c);
    T y = (a * y1 + b * y2 + c * y3) / (a + b + c);
    T r = area / s;
    return {x, y, r};
}
