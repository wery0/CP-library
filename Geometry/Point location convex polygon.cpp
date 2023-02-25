/*
{-2, -2} - outside
{-1, -1} - inside
{i, i} (i >= 0) - border, p == ch[i]
{i, j} (i, j >= 0, i != j) - border, p on segment {ch[i], ch[j]}
*/
//Requirement: ch - points of convex hull of polygon, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested with point_location_convex_polygon_slow
template<typename T>
pair<int, int> point_location_convex_polygon(const vector<pt<T>>& ch, const pt<T>& p) {
    static constexpr pair<int, int> OUTSIDE = {-2, -2};
    static constexpr pair<int, int> INSIDE = {-1, -1};
    const int n = ch.size();
    auto pis = [&](const pt<T>& p1, const pt<T>& p2, const pt<T>& p) -> bool {
        return dot(p2 - p1, p - p1) >= 0 && dot(p1 - p2, p - p2) >= 0;
    };
    if (n == 0) return OUTSIDE;
    if (n == 1) return ch[0] == p ? make_pair(0, 0) : OUTSIDE;
    if (n == 2) {
        if (cross(ch[0] - p, ch[1] - p) == 0 && pis(ch[0], ch[1], p)) {
            if (p == ch[0]) return {0, 0};
            if (p == ch[1]) return {1, 1};
            return {0, 1};
        }
        return OUTSIDE;
    }
    if (p.x < ch[0].x) return OUTSIDE;
    if (cross(p - ch[0], ch[1] - ch[0]) > 0) return OUTSIDE;
    if (cross(ch.back() - ch[0], p - ch[0]) > 0) return OUTSIDE;
    int l = 1, r = n;
    while (l + 1 < r) {
        int md = (l + r) / 2;
        (cross(ch[md] - ch[0], p - ch[0]) >= 0 ? l : r) = md;
    }
    if (cross(ch[l] - ch[0], p - ch[0]) == 0) {
        if (p == ch[l]) return {l, l};
        if (pis(ch[0], ch[l], p)) {
            if (l == 1 || l == n - 1) {
                if (ch[0] == p) return {0, 0};
                if (ch[l] == p) return make_pair(l, l);
                return l == 1 ? make_pair(0, l) : make_pair(l, 0);
            }
            return INSIDE;
        }
        return OUTSIDE;
    }
    T val = cross(ch[l + 1] - p, p - ch[l]);
    if (val) return val < 0 ? OUTSIDE : INSIDE;
    return {l, (l + 1) % n};
}