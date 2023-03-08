/*
{-2, -2} - p is outside
{-1, -1} - p is inside
{i, i} (i >= 0) - p is on the border, p == ch[i]
{i, j} (i, j >= 0, i != j) - p is on the border, p lies inside segment {ch[i], ch[j]}
*/
//Requirement: ch - points of strict convex hull, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested with linear version of this function
template<typename T, typename U>
array<ssize_t, 2> point_location_convex_polygon(const vector<pt<T>>& ch, const pt<U>& p) {
    static const auto eps = is_integral_v<T> && is_integral_v<U> ? (T)0 : is_floating_point_v<T> ? (T)EPS : (U)EPS;
    static constexpr array<ssize_t, 2> OUTSIDE = {-2, -2};
    static constexpr array<ssize_t, 2> INSIDE = {-1, -1};
    const ssize_t n = ch.size();
    auto pis = [&](const pt<T>& p1, const pt<T>& p2) -> bool {
        return dot(p2 - p1, p - p1) >= -eps && dot(p1 - p2, p - p2) >= -eps;
    };
    if (n == 0) return OUTSIDE;
    if (n == 1) {
        if (ch[0] == p) return {0, 0};
        return OUTSIDE;
    }
    if (n == 2) {
        if (abs(cross(ch[0] - p, ch[1] - p)) <= eps && pis(ch[0], ch[1])) {
            if (p == ch[0]) return {0, 0};
            if (p == ch[1]) return {1, 1};
            return {0, 1};
        }
        return OUTSIDE;
    }
    if (p.x + eps < ch[0].x) return OUTSIDE;
    if (cross(p - ch[0], ch[1] - ch[0]) > eps) return OUTSIDE;
    if (cross(ch.back() - ch[0], p - ch[0]) > eps) return OUTSIDE;
    ssize_t l = 1, r = n;
    while (l + 1 < r) {
        ssize_t md = (l + r) / 2;
        (cross(ch[md] - ch[0], p - ch[0]) >= -eps ? l : r) = md;
    }
    if (abs(cross(ch[l] - ch[0], p - ch[0])) <= eps) {
        if (p == ch[l]) return {l, l};
        if (pis(ch[0], ch[l])) {
            if (l == 1 || l == n - 1) {
                if (ch[0] == p) return {0, 0};
                if (ch[l] == p) return {l, l};
                if (l == 1) return {0, l};
                return {l, 0};
            }
            return INSIDE;
        }
        return OUTSIDE;
    }
    auto val = cross(ch[l + 1] - p, p - ch[l]);
    if (abs(val) > eps) return val < 0 ? OUTSIDE : INSIDE;
    return {l, (l + 1) % n};
}