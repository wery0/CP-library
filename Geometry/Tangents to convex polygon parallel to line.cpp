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
template<typename T>
pair<pair<int, int>, pair<int, int>> get_tangents_to_convex_polygon_parallel_to_line(const vector<pt<T>>& ch, const line<T>& l) {
    const size_t n = ch.size();
    if (n == 0) return {{-1, -1}, {-1, -1}};
    if (n == 1) return {{0, 0}, {0, 0}};
    auto cmp = [&](const auto & l, const auto & r) -> bool {
        int pl = l.x > 0 ? 0 : !l.x && l.y >= 0 ? 1 : 2;
        int pr = r.x > 0 ? 0 : !r.x && r.y >= 0 ? 1 : 2;
        return pl != pr ? pl < pr : l.x * r.y - l.y * r.x > 0;
    };
    auto get_side = [&](size_t i) -> pt<T> {return ch[(i + 1) % n] - ch[i];};
    auto binsearch = [&](const pt<T>& v) -> size_t {
        ssize_t l = -1, r = n;
        while (l + 1 < r) {
            ssize_t md = l + (r - l) / 2;
            (cmp(get_side(md), v) ? l : r) = md;
        }
        return r % n;
    };
    pt<T> v1(l.B, -l.A), v2 = -v1;
    size_t p1 = binsearch(v1);
    size_t p2 = binsearch(v2);
    pair<int, int> ans1 = {p1, p1};
    pair<int, int> ans2 = {p2, p2};
    if (cross(get_side(p1), v1) == 0) ans1.second = (ans1.second + 1) % n;
    if (cross(get_side(p2), v2) == 0) ans2.second = (ans2.second + 1) % n;
    if (p1 > p2) swap(ans1, ans2);
    return {ans1, ans2};
}