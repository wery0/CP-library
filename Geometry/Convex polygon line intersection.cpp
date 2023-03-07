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
    const size_t n = ch.size();
    auto eval = [&](size_t i) -> U {i -= i >= n ? n : 0; return l.A * ch[i].x + l.B * ch[i].y + l.C;};
    auto sign = [](U x) -> int {return x < -EPS ? -1 : x > EPS ? 1 : 0;};
    auto is_point_on_segment = [](const pt<D>& p, const pt<D>& p1, const pt<D>& p2) -> bool {
        return abs(cross(p - p1, p - p2)) < EPS && dot(p2 - p1, p - p1) > -EPS && dot(p1 - p2, p - p2) > -EPS;
    };
    auto segment_line_intersection = [&](const pt<T>& p1, const pt<T>& p2) -> vector<pt<D>> {
        line u(p1, p2);
        if (l.is_parallel_to(u)) {
            if (l.is_equal_to(u)) return {p1, p2, p1};
            return {};
        }
        pt<D> p = l.intersect(u);
        if (is_point_on_segment(p, p1, p2)) return {p};
        return {};
    };
    if (n == 0) return {};
    if (n == 1) {
        if (l.get_dist_to_pt(ch[0]) < EPS) return {ch[0]};
        return {};
    }
    if (n == 2) return segment_line_intersection(ch[0], ch[1]);
    auto [t1, t2] = get_tangents_to_convex_polygon_parallel_to_line(ch, l);
    T c1 = -l.A * ch[t1.first].x - l.B * ch[t1.first].y;
    T c2 = -l.A * ch[t2.first].x - l.B * ch[t2.first].y;
    if (c1 > c2) swap(c1, c2), swap(t1, t2);
    if (l.C < c1 - EPS || c2 + EPS < l.C) return {};
    for (auto [t, c] : {pair{t1, c1}, pair{t2, c2}}) {
        if (abs(l.C - c) < EPS) {
            if (t.first == t.second) return {ch[t.first]};
            return {ch[t.first], ch[t.second], ch[t.first]};
        }
    }
    auto binsearch = [&](size_t lf, size_t rg) -> pt<D> {
        int lsign = sign(eval(lf));
        while (lf + 1 < rg) {
            size_t md = lf + (rg - lf) / 2;
            int tyt = sign(eval(md));
            (tyt == lsign ? lf : rg) = md;
        }
        auto v = segment_line_intersection(ch[lf % n], ch[rg % n]);
        assert(v.size() == 1);
        return v[0];
    };
    if (t2 < t1) swap(t1, t2);
    size_t l1 = t1.second, r1 = t2.first;
    size_t l2 = t2.second, r2 = t1.first;
    if (l2 > r2) r2 += n;
    pt<D> p1 = binsearch(l1, r1);
    pt<D> p2 = binsearch(l2, r2);
    return {p1, p2};
}