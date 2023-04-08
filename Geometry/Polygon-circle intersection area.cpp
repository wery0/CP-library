template<typename T>
T triangle_circle_intersection(pt<T> c, T r, pt<T> a, pt<T> b) {
    T sd1 = dist2(c, a), sd2 = dist2(c, b);
    if (sd1 > sd2) swap(a, b), swap(sd1, sd2);
    T sd = dist2(a, b);
    T d1 = sqrtl(sd1), d2 = sqrtl(sd2), d = sqrtl(sd);
    T x = abs(sd2 - sd - sd1) / (2 * d);
    T h = sqrtl(sd1 - x * x);
    if (r >= d2) return h * d / 2;
    T area = 0;
    if (sd + sd1 < sd2) {
        if (r < d1) area = r * r * (acosl(h / d2) - acosl(h / d1));
        else {
            area = r * r * (acosl(h / d2) - acosl(h / r));
            T y = sqrtl(r * r - h * h);
            area += h * (y - x);
        }
    } else {
        if (r < h) area = r * r * (acosl(h / d2) + acosl(h / d1));
        else {
            area += r * r * (acosl(h / d2) - acosl(h / r));
            T y = sqrtl(r * r - h * h);
            area += h * y;
            if (r < d1) {
                area += r * r * (acosl(h / d1) - acosl(h / r));
                area += h * y;
            } else area += h * x;
        }
    }
    return area / 2;
}

template<typename T>
T polygon_circle_intersection(const vector<pt<T>>& v, const pt<T>& p, const T& r) {
    auto orientation = [](const pt<T>& a, const pt<T>& b, const pt<T>& c) -> int{
        const auto val = cross(b - a, c - a);
        return val < 0 ? -1 : val > 0 ? 1 : 0;
    };
    const int n = v.size();
    T ans = 0;
    pt<T> org = {0, 0};
    for (int i = 0; i < n; i++) {
        int x = orientation(p, v[i], v[(i + 1) % n]);
        T area = triangle_circle_intersection(org, r, v[i] - p, v[(i + 1) % n] - p);
        ans += area * x;
    }
    return abs(ans);
}