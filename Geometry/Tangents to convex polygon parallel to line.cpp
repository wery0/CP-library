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
pair<pair<int, int>, pair<int, int>> get_tangents_to_convex_polygon_parallel_to_line(const vector<pt<T>>& ch, const line<U>& l) {
    const U eps = is_integral_v<U> ? 0 : EPS;
    const size_t n = ch.size();
    if (n == 0) return {{-1, -1}, {-1, -1}};
    if (n == 1) return {{0, 0}, {0, 0}};
    auto cmp = [&](const pt<T>& l, const pt<U>& r) -> bool {
        int pl = l.x > 0 ? 0 : l.x == 0 && l.y >= 0 ? 1 : 2;
        int pr = r.x > eps ? 0 : abs(r.x) <= eps && r.y >= -eps ? 1 : 2;
        return pl != pr ? pl < pr : cross(l, r) > eps;
    };
    auto get_side = [&](size_t i) -> pt<T> {return ch[(i + 1) % n] - ch[i];};
    auto binsearch = [&](const pt<U>& v) -> size_t {
        ssize_t l = -1, r = n;
        while (l + 1 < r) {
            ssize_t md = l + (r - l) / 2;
            (cmp(get_side(md), v) ? l : r) = md;
        }
        return r % n;
    };
    pt<U> v1(l.B, -l.A), v2 = -v1;
    size_t p1 = binsearch(v1);
    size_t p2 = binsearch(v2);
    pair<int, int> ans1 = {p1, p1};
    pair<int, int> ans2 = {p2, p2};
    if (abs(cross(get_side(p1), v1)) <= eps) ans1.second = (ans1.second + 1) % n;
    if (abs(cross(get_side(p2), v2)) <= eps) ans2.second = (ans2.second + 1) % n;
    if (p1 > p2) swap(ans1, ans2);
    return {ans1, ans2};
}