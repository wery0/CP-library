//T - type of point coordinates
//R - type of radius
template<typename T, typename R = long double, typename D = long double>
class circle {
    static_assert(is_floating_point_v<D>);
    static constexpr D PI = 3.14159265358979323846264338327950288419716939937510;

    void tangents(pt<T> c, R r1, R r2, vector<line<T>>& ans) {
        R r = r2 - r1;
        D z = c.x * c.x + c.y * c.y;
        D d = z - r * r;
        if (d < -EPS) return;
        d = sqrtl(abs(d));
        ans.emplace_back((c.x * r + c.y * d) / z, (c.y * r - c.x * d) / z, r1);
    }

public:
    pt<T> p;
    R r;

    circle() = default;
    circle(pt<T> p, R r): p(p), r(r) {}

    vector<pt<T>> line_intersection(line<T>& l) {
        D dst = l.get_dist_to_pt(p);
        if (dst > r + EPS) return {};
        pt<T> mid = l.intersect(l.get_normal_from_point(p));
        if (abs(dst - r) < EPS) return {mid};
        pt<T> n = {-l.B, l.A};
        D d = sqrtl(r * r - dst * dst);
        n.self_normalize();
        return {mid - n * d, mid + n * d};
    }

    vector<pt<T>> circle_intersection(circle<T>& c) {
        D d = dist(c.p, p);
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
            for (const auto& u : nw) fl |= u.is_equal_to(l);
            if (!fl) nw.push_back(l);
        }
        return nw;
    }

    friend istream& operator>>(istream& is, circle& c) {return is >> c.p >> c.r;}
};

template<typename T, typename D = long double>
D calc_len_of_arc(const pt<T>& c, const pt<T>& p1, const pt<T>& p2) {
    T rr = dist2(c, p1);
    T dd = dist2(p1, p2);
    D arg = (D)1 - (D)dd / (2 * rr);
    D ang = acosl(abs(abs(arg) - 1) < EPS ? arg < 0 ? -1 : 1 : arg);
    return sqrtl(rr) * ang;
}
