const ld PI = 3.14159265358979323846264338327950288419716939937510;

template<typename T = ld>
struct circle {

    pt<T> p;
    ld r;

    circle() {}
    circle(pt<T> a, ld b): p(a), r(b) {}

    vec<pt<T>> line_intersection(line<T> l) {
        ld dst = l.dst_to_pt(p);
        if (dst > r + EPS) return {};
        pt<T> mid = l.intersect(l.get_normal(p));
        if (abs(dst - r) < EPS) return {mid};
        pt<T> n = { -l.B, l.A};
        ld d = sqrtl(r * r - dst * dst);
        n.normalize();
        return {mid - n * d, mid + n * d};
    }

    vec<pt<T>> circle_intersection(circle<T> c) {
        ld d = c.p.dst(p);
        if (abs(d) < EPS) {
            if (abs(r - c.r) > EPS) return {};
            //process inf points
            assert(0);
            return {};
        }
        pt<T> df = p;
        p -= df, c.p -= df;
        line l = line(-2 * c.p.x, -2 * c.p.y, c.p.dot(c.p) + r * r - c.r * c.r);
        vec<pt<T>> m = line_intersection(l);
        for (auto &p : m) p += df;
        p += df;
        c.p += df;
        return m;
    }

    void tangents(pt<T> c, ld r1, ld r2, vec<line<T>> &ans) {
        ld r = r2 - r1;
        ld z = c.x * c.x + c.y * c.y;
        ld d = z - r * r;
        if (d < -EPS) return;
        d = sqrtl(abs(d));
        ans.pb(line((c.x * r + c.y * d) / z, (c.y * r - c.x * d) / z, r1));
    }

    vec<line<T>> common_tangents (circle<T> b) {
        vec<line<T>> ans;
        tangents(b.p - p, r, b.r, ans);
        tangents(b.p - p, r, -b.r, ans);
        tangents(b.p - p, -r, b.r, ans);
        tangents(b.p - p, -r, -b.r, ans);
        for (auto &l : ans) l.C -= l.A * p.x + l.B * p.y;
        vec<line<T>> nw;
        for (line l : ans) {
            int fl = 0;
            for (auto &u : nw) fl |= u.is_equal(l);
            if (!fl) nw.pb(l);
        }
        return nw;
    }
};

template<typename T>
T calc_len_of_arc(pt<T> c, pt<T> p1, pt<T> p2) {
    T r = dst(c, p1);
    T d = dst(p1, p2);
    T ang = acosl(1 - d * d / (2 * r * r) + EPS);
    return r * ang;
}