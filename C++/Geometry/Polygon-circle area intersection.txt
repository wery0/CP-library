int sign(ld x) { return (x > EPS) - (x < -EPS); }
inline ld dot(pt<ld> a, pt<ld> b) { return a.x * b.x + a.y * b.y; }
inline ld dist2(pt<ld> a, pt<ld> b) { return dot(a - b, a - b); }
inline ld cross(pt<ld> a, pt<ld> b) { return a.x * b.y - a.y * b.x; }
inline int orientation(pt<ld> a, pt<ld> b, pt<ld> c) { return sign(cross(b - a, c - a)); }

ld triangle_circle_intersection(pt<ld> c, ld r, pt<ld> a, pt<ld> b) {
    ld sd1 = dist2(c, a), sd2 = dist2(c, b);
    if (sd1 > sd2) swap(a, b), swap(sd1, sd2);
    ld sd = dist2(a, b);
    ld d1 = sqrtl(sd1), d2 = sqrtl(sd2), d = sqrtl(sd);
    ld x = abs(sd2 - sd - sd1) / (2 * d);
    ld h = sqrtl(sd1 - x * x);
    if (r >= d2) return h * d / 2;
    ld area = 0;
    if (sd + sd1 < sd2) {
        if (r < d1) area = r * r * (acosl(h / d2) - acosl(h / d1));
        else {
            area = r * r * (acosl(h / d2) - acosl(h / r));
            ld y = sqrtl(r * r - h * h);
            area += h * (y - x);
        }
    } else {
        if (r < h) area = r * r * (acosl(h / d2) + acosl(h / d1));
        else {
            area += r * r * (acosl(h / d2) - acosl(h / r));
            ld y = sqrtl(r * r - h * h);
            area += h * y;
            if (r < d1) {
                area += r * r * (acosl(h / d1) - acosl(h / r));
                area += h * y;
            } else area += h * x;
        }
    }
    return area / 2;
}

ld polygon_circle_intersection(vec<pt<ld>> &v, pt<ld> p, ld r) {
    int n = v.size();
    ld ans = 0;
    pt<ld> org = {0, 0};
    for (int i = 0; i < n; i++) {
        int x = orientation(p, v[i], v[(i + 1) % n]);
        ld area = triangle_circle_intersection(org, r, v[i] - p, v[(i + 1) % n] - p);
        ans += area * x;
    }
    return abs(ans);
}