/*
{} - no intersection
{p_1} - intersection at point p_1
{p_1, p_2} - intersection at points p_1, p_2
{p_1, p_2, p_1} - intersection along the edge (p_1, p_2)
*/
//Requirement: ch - points of strict convex hull, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested with linear version of this function
template<typename T, typename U, typename D = long double>
vector<pt<D>> intersection_convex_polygon_line(const vector<pt<T>>& ch, const line<U>& l) {
    static_assert(is_floating_point_v<D>);
    static const auto eps = is_integral_v<T> && is_integral_v<U> ? (T)0 : is_floating_point_v<T> ? (T)EPS : (U)EPS;
    using H = remove_const<decltype(eps)>::type;
    const ssize_t n = ch.size();
    auto sign = [](H x) -> int {return x < -eps ? -1 : x > eps ? 1 : 0;};
    auto segment_line_intersection = [&](const pt<T>& p1, const pt<T>& p2) -> vector<pt<D>> {
        line<T> u(p1, p2);
        if (l.is_parallel_to(u)) {
            if (l.is_equal_to(u)) return {p1, p2, p1};
            return {};
        }
        int s1 = sign(l.eval(p1));
        int s2 = sign(l.eval(p2));
        if (s1 == 0) return {p1};
        if (s2 == 0) return {p2};
        if (s1 == s2) return {};
        pt<D> p = l.intersect(u);
        return {p};
    };
    if (n == 0) return {};
    if (n == 1) {
        if (l.get_dist_to_pt(ch[0]) < EPS) return {ch[0]};
        return {};
    }
    if (n == 2) return segment_line_intersection(ch[0], ch[1]);
    auto [t1, t2] = get_tangents_to_convex_polygon_parallel_to_line(ch, l);
    H c1 = -l.A * ch[t1[0]].x - l.B * ch[t1[0]].y;
    H c2 = -l.A * ch[t2[0]].x - l.B * ch[t2[0]].y;
    if (c1 > c2) swap(c1, c2), swap(t1, t2);
    if (l.C < c1 - eps || c2 + eps < l.C) return {};
    for (auto [t, c] : {pair{t1, c1}, pair{t2, c2}}) {
        if (abs(l.C - c) <= eps) {
            if (t[0] == t[1]) return {ch[t[0]]};
            return {ch[t[0]], ch[t[1]], ch[t[0]]};
        }
    }
    auto binsearch = [&](ssize_t lf, ssize_t rg) -> pt<D> {
        int lsign = sign(l.eval(ch[lf >= n ? lf - n : lf]));
        while (lf + 1 < rg) {
            ssize_t md = lf + (rg - lf) / 2;
            int tyt = sign(l.eval(ch[md >= n ? md - n : md]));
            (tyt == lsign ? lf : rg) = md;
        }
        auto v = segment_line_intersection(ch[lf >= n ? lf - n : lf], ch[rg >= n ? rg - n : rg]);
        assert(v.size() == 1);
        return v[0];
    };
    if (t2 < t1) swap(t1, t2);
    ssize_t l1 = t1[1], r1 = t2[0];
    ssize_t l2 = t2[1], r2 = t1[0];
    if (l2 > r2) r2 += n;
    pt<D> p1 = binsearch(l1, r1);
    pt<D> p2 = binsearch(l2, r2);
    return {p1, p2};
}
