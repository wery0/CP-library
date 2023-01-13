const ld PI = 3.14159265358979323846264338327950288419716939937510;

template<typename T = ld>
class circle {
    void tangents(pt<T> c, ld r1, ld r2, vector<line<T>>& ans) {
        ld r = r2 - r1;
        ld z = c.x * c.x + c.y * c.y;
        ld d = z - r * r;
        if (d < -EPS) return;
        d = sqrtl(abs(d));
        ans.emplace_back((c.x * r + c.y * d) / z, (c.y * r - c.x * d) / z, r1);
    }

public:
    pt<T> p;
    ld r;

    circle() {}
    circle(pt<T> p, ld r): p(p), r(r) {}

    vector<pt<T>> line_intersection(line<T>& l) {
        ld dst = l.get_dst_to_pt(p);
        if (dst > r + EPS) return {};
        pt<T> mid = l.intersect(l.get_normal_from_point(p));
        if (abs(dst - r) < EPS) return {mid};
        pt<T> n = { -l.B, l.A};
        ld d = sqrtl(r * r - dst * dst);
        n.self_normalize();
        return {mid - n * d, mid + n * d};
    }

    vector<pt<T>> circle_intersection(circle<T>& c) {
        ld d = dst(c.p, p);
        if (abs(d) < EPS) {
            if (abs(r - c.r) > EPS) return {};
            //process inf points
            assert(0);
            return {};
        }
        pt<T> df = p;
        p -= df, c.p -= df;
        line l = line(-2 * c.p.x, -2 * c.p.y, dot(c.p, c.p) + r * r - c.r * c.r);
        vector<pt<T>> ans = line_intersection(l);
        for (auto& p : ans) p += df;
        p += df, c.p += df;
        return ans;
    }

    vector<line<T>> common_tangents(circle<T>& b) {
        vector<line<T>> ans;
        tangents(b.p - p, r, b.r, ans);
        tangents(b.p - p, r, -b.r, ans);
        tangents(b.p - p, -r, b.r, ans);
        tangents(b.p - p, -r, -b.r, ans);
        for (auto& l : ans) l.C -= l.A * p.x + l.B * p.y;
        vector<line<T>> nw;
        for (auto& l : ans) {
            int fl = 0;
            for (auto& u : nw) fl |= u.is_equal_to(l);
            if (!fl) nw.push_back(l);
        }
        return nw;
    }

    friend istream& operator>>(istream& is, circle& c) {return is >> c.p >> c.r;}
};

template<typename T>
T calc_len_of_arc(pt<T> c, pt<T> p1, pt<T> p2) {
    T rr = sqdst(c, p1);
    T dd = sqdst(p1, p2);
    T ang = acosl(1 - dd / (2 * rr) + EPS);
    return sqrtl(rr) * ang;
}