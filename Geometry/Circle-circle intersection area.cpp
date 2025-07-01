template<typename T>
T circle_circle_intersection(circle<T> c1, circle<T> c2) {
    auto GERON = [&](T a, T b, T c) -> T{
        T p = (a + b + c) / 2;
        return sqrtl(p * (p - a) * (p - b) * (p - c));
    };
    auto FU = [&](T r, T d) -> T{
        return r * r * acos(d / r) - d * sqrtl(r * r - d * d);
    };
    if (c1.r > c2.r) swap(c1, c2);
    T d = dist(c1.p, c2.p);
    if (d >= c1.r + c2.r) return 0;
    if (c1.r + d <= c2.r) return PI * c1.r * c1.r;
    T A = sqrtl(4 * d * d * c2.r * c2.r - powl(d * d - c1.r * c1.r + c2.r * c2.r, 2)) / d;
    if (c2.r <= d) {
        T SQ = GERON(c1.r, c2.r, d) * 2, t = (SQ * 2) / d;
        T A1 = acos(1 - (t * t) / (2 * c1.r * c1.r));
        T A2 = acos(1 - (t * t) / (2 * c2.r * c2.r));
        T p1 = (c1.r * c1.r * A1) / 2 - GERON(c1.r, c1.r, t);
        T p2 = (c2.r * c2.r * A2) / 2 - GERON(c2.r, c2.r, t);
        return p1 + p2;
    }
    T d1 = (d * d - c1.r * c1.r + c2.r * c2.r) / (2 * d);
    T d2 = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
    return FU(c2.r, d1) + FU(c1.r, d2);
}
