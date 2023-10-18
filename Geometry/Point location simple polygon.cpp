//-1 - outside
//0 - border
//1 - inside
//O(n)
template<typename T>
int point_location_simple_polygon(const vector<pt<T>>& poly, const pt<T>& p) {
    static uniform_real_distribution<double> gen(-3.0, 3.0);
    static mt19937 rnd;
    static const auto eps = is_integral_v<T> ? 0 : EPS;
    auto sign = [](T x) -> int {return x < -eps ? -1 : x > eps ? 1 : 0;};
    auto pis = [&](const pt<T>& p1, const pt<T>& p2) -> bool {
        return abs(cross(p1 - p, p2 - p)) <= eps && dot(p2 - p1, p - p1) >= -eps && dot(p1 - p2, p - p2) >= -eps;
    };
    const size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        if (pis(poly[i], poly[(i + 1) % n])) return 0;
    }
    T mny = numeric_limits<T>::max();
    T mxy = numeric_limits<T>::min();
    for (const auto& [x, y] : poly) {
        mny = min(mny, y);
        mxy = max(mxy, y);
    }
    if (p.y + eps <= mny || p.y - eps >= mxy) return -1;
    pt<T> v = {1, mxy - mny};
    for (int it = 0; it < 10; ++it) {
        if (is_floating_point_v<T>) v.self_rotate_about_origin(gen(rnd));
        pt<T> p2 = p + v;
        size_t cnt = 0;
        bool is_good_angle = true;
        for (size_t i = 0; i < n; ++i) {
            pt<T> p3 = poly[i];
            pt<T> p4 = poly[(i + 1) % n];
            int s1 = sign(cross(p2 - p, p3 - p));
            int s2 = sign(cross(p2 - p, p4 - p));
            if (s1 == 0 || s2 == 0) {
                assert(is_floating_point_v<T> && "Overflow?");
                is_good_angle = false;
                break;
            }
            if (s1 != s2) {
                int s3 = sign(cross(p4 - p3, p - p3));
                int s4 = sign(cross(p4 - p3, p2 - p3));
                assert(s3 != 0 && s4 != 0);
                cnt += s3 != s4;
            }
        }
        if (is_good_angle) return cnt & 1 ? 1 : -1;
    }
    assert(0 && "Something went wrong");
    return 0;
}
