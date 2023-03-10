/*
{{-1, -1}, {-1, -1}} - no tangents (only when n == 0)
{P1, P2}, where P1 = {A, B}, P2 = {C, D}
    if A == B then
        parallel line passes only through ch[A]
    else
        B = (A + 1) % n and the parallel line is line(ch[A], ch[B])
Same logic applies for P2.
*/
//Requirement: ch - points of strict convex hull, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested with linear version of this function
//Note: Be careful with cases 0 <= n <= 2
template<typename T, typename U>
array<array<ssize_t, 2>, 2> get_tangents_to_convex_polygon_parallel_to_line(const vector<pt<T>>& ch, const line<U>& l) {
    static const auto eps = is_integral_v<T> && is_integral_v<U> ? (T)0 : is_floating_point_v<T> ? (T)EPS : (U)EPS;
    const ssize_t n = ch.size();
    if (n == 0) return {array<ssize_t, 2>{-1, -1}, array<ssize_t, 2>{-1, -1}};
    if (n == 1) return {array<ssize_t, 2>{0, 0}, array<ssize_t, 2>{0, 0}};
    auto cmp = [&](const pt<T>& l, const pt<U>& r) -> bool {
        int pl = l.x > eps ? 0 : abs(l.x) <= eps && l.y >= -eps ? 1 : 2;
        int pr = r.x > eps ? 0 : abs(r.x) <= eps && r.y >= -eps ? 1 : 2;
        return pl != pr ? pl < pr : cross(l, r) > eps;
    };
    auto get_side = [&](ssize_t i) -> pt<T> {return ch[i == n - 1 ? 0 : i + 1] - ch[i];};
    auto binsearch = [&](const pt<U>& v) -> ssize_t {
        ssize_t l = -1, r = n;
        while (l + 1 < r) {
            ssize_t md = l + (r - l) / 2;
            (cmp(get_side(md), v) ? l : r) = md;
        }
        return r % n;
    };
    pt<U> v1(l.B, -l.A), v2 = -v1;
    ssize_t p1 = binsearch(v1);
    ssize_t p2 = binsearch(v2);
    array<ssize_t, 2> ans1 = {p1, p1};
    array<ssize_t, 2> ans2 = {p2, p2};
    if (abs(cross(get_side(p1), v1)) <= eps) ans1[1] = ans1[1] == n - 1 ? 0 : ans1[1] + 1;
    if (abs(cross(get_side(p2), v2)) <= eps) ans2[1] = ans2[1] == n - 1 ? 0 : ans2[1] + 1;
    if (p1 > p2) swap(ans1, ans2);
    return {ans1, ans2};
}