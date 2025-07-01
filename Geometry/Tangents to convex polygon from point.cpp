/*
{-1, -1} - no tangents (when n == 0 or n == 1 and p == ch[0] or p is strictly inside polygon)
{A, B} (0 <= A, B < n) - tangents are line(p, ch[A]), line(p, ch[B])
    if A < B then
        the points ch[A], ch[A + 1], ..., ch[B]
    else if A > B
        the points ch[B], ch[(B + 1) % n], ..., ch[n - 1], ch[0], ..., ch[A]
    on the same side from line(ch[A], ch[B]) as point p

If line(p, ch[A]) contains some ch[T] (T != A), then 'furthest_points' decides, will A be reptaced with T or not.
The same logic with B.
*/
//Requirement: ch - points of strict convex hull, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested
//Note: Handle cases 0 <= n <= 2 differently, if need
template<typename T, typename U>
array<ssize_t, 2> get_tangents_to_convex_polygon_from_point(const vector<pt<T>>& ch, const pt<U>& p,
        bool furthest_points = false, array<ssize_t, 2> location = {-3, -3}) {
    static const auto eps = is_integral_v<T> && is_integral_v<U> ? (T)0 : is_floating_point_v<T> ? (T)EPS : (U)EPS;
    using H = remove_const<decltype(eps)>::type;
    static constexpr array<ssize_t, 2> NO = {-1, -1};
    auto sign = [](H x) -> int {return x < -eps ? -1 : x > eps ? 1 : 0;};
    const ssize_t n = ch.size();
    if (location[0] == -3) location = point_location_convex_polygon(ch, p);
    if (location[0] == -1) return NO;
    if (location[0] >= 0) {
        if (location[0] != location[1]) return location;
        return {(location[0] + n - 1) % n, (location[1] + 1) % n};
    }
    if (n == 0) return NO;
    if (n == 1) {
        if (p == ch[0]) return NO;
        return {0, 0};
    }
    if (n == 2) {
        if (cross(ch[0] - p, ch[1] - p) == 0) {
            auto d = dot(ch[0] - p, ch[1] - ch[0]);
            if (d == 0) {
                if (p == ch[0]) return {1, 1};
                if (p == ch[1]) return {0, 0};
            }
            return {(d <= eps) ^ furthest_points, (d <= eps) ^ furthest_points};
        } else return {0, 1};
    }
    line<H> l(p, ch[0]);
    ssize_t lf = 0, rg = n;
    int sgn = sign(l.eval(ch[1]));
    if (sgn == 0) {
        lf = 1;
    } else {
        while (lf + 1 < rg) {
            ssize_t md = (lf + rg) / 2;
            int tyt = sign(l.eval(ch[md]));
            if (tyt == -sgn) rg = md;
            else lf = md;
        }
    }
    auto go = [&](ssize_t l, ssize_t r) -> ssize_t {
        int st = sign(cross(ch[l] - p, ch[(l + 1) % n] - p));
        while (l + 1 < r) {
            ssize_t md = (l + r) / 2;
            int tyt = sign(cross(ch[md] - p, ch[(md + 1) % n] - p));
            if (tyt == st) l = md;
            else r = md;
        }
        if (sign(cross(ch[l] - p, ch[(l + 1) % n] - p)) == 0) {
            return mdist(p, ch[l]) < mdist(p, ch[(l + 1) % n]) ? l : (l + 1) % n;
        } else if (sign(cross(ch[(l + 1) % n] - p, ch[(l + 2) % n] - p)) == 0) {
            return mdist(p, ch[(l + 1) % n]) < mdist(p, ch[(l + 2) % n]) ? (l + 1) % n : (l + 2) % n;
        } else return (l + 1) % n;
    };
    ssize_t p1 = go(0, lf);
    ssize_t p2 = go(lf, n);
    int need_sign = sign(cross(ch[p2] - ch[p1], p - ch[p1]));
    ssize_t np = (p1 + 1) % n;
    if (np == p2) {
        swap(p1, p2);
        need_sign *= -1;
        np = (p1 + 1) % n;
    }
    int tyt_sign = sign(cross(ch[p2] - ch[p1], ch[np] - ch[p1]));
    if (tyt_sign != need_sign) swap(p1, p2);
    if (furthest_points) {
        if (abs(cross(ch[p1] - p, ch[(p1 + n - 1) % n] - p)) <= eps) p1 = (p1 + n - 1) % n;
        if (abs(cross(ch[p2] - p, ch[(p2 + 1) % n] - p)) <= eps) p2 = (p2 + 1) % n;
    }
    return {p1, p2};
}
