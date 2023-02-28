/*
{-1, -1} - no tangents
{A, B} (0 <= A, B < n) - tangents are line(p, ch[A]), line(p, ch[B])
    if A < B then
        the points ch[A], ch[A + 1], ..., ch[B]
    else if A > B
        the points ch[B], ch[(B + 1) % n], ..., ch[n - 1], ch[0], ..., ch[A]
    on the same side from line(ch[A], ch[B]) as point p

If line(p, ch[A]) contains some ch[T] (T != A), then 'furthest_points' decides, will A be reptaced with T or not.
The same logic with B.
*/
//Requirement: ch - points of convex hull of polygon, ordered in CCW order, starting from the lowest of the leftmost points.
//Complexity: O(log(n))
//Correctness: Stress tested
//Note: Handle cases 0 <= n <= 2 differently, if need
template<typename T>
pair<int, int> get_tangents_to_polygon(const vector<pt<T>>& ch, const pt<T>& p,
                                       pair<int, int> location = {-3, -3}, bool furthest_points = false) {
    static constexpr pair<int, int> NO = {-1, -1};
    auto sign = [](T x) -> int {return x < 0 ? -1 : x > 0 ? 1 : 0;};
    auto eval = [](const line<T>& l, const pt<T>& p) -> T {return l.A * p.x + l.B * p.y + l.C;};
    const int n = ch.size();
    if (location.first == -3) location = point_location_convex_polygon(ch, p);
    if (location.first == -1) return NO;
    if (location.first >= 0) {
        if (location.first != location.second) return location;
        return {(location.first + n - 1) % n, (location.second + 1) % n};
    }
    if (n == 0) return NO;
    if (n == 1) return p == ch[0] ? NO : make_pair(0, 0);
    if (n == 2) {
        if (cross(ch[0] - p, ch[1] - p) == 0) {
            T d = dot(ch[0] - p, ch[1] - ch[0]);
            if (d == 0) {
                if (p == ch[0]) return {1, 1};
                if (p == ch[1]) return {0, 0};
            }
            return (d > 0) ^ furthest_points ? make_pair(0, 0) : make_pair(1, 1);
        } else return {0, 1};
    }
    line<T> l(p, ch[0]);
    int lf = 0, rg = n;
    int sgn = sign(eval(l, ch[1]));
    if (sgn == 0) {
        lf = 1;
    } else {
        while (lf + 1 < rg) {
            int md = (lf + rg) / 2;
            int tyt = sign(eval(l, ch[md]));
            if (tyt == -sgn) rg = md;
            else lf = md;
        }
    }
    auto go = [&](int l, int r) -> int {
        int st = sign(cross(ch[l] - p, ch[(l + 1) % n] - p));
        while (l + 1 < r) {
            int md = (l + r) / 2;
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
    int p1 = go(0, lf);
    int p2 = go(lf, n);
    int need_sign = sign(cross(ch[p2] - ch[p1], p - ch[p1]));
    int np = (p1 + 1) % n;
    if (np == p2) {
        swap(p1, p2);
        need_sign *= -1;
        np = (p1 + 1) % n;
    }
    int tyt_sign = sign(cross(ch[p2] - ch[p1], ch[np] - ch[p1]));
    if (tyt_sign != need_sign) swap(p1, p2);
    if (furthest_points) {
        if (cross(ch[p1] - p, ch[(p1 + n - 1) % n] - p) == 0) p1 = (p1 + n - 1) % n;
        if (cross(ch[p2] - p, ch[(p2 + 1) % n] - p) == 0) p2 = (p2 + 1) % n;
    }
    return {p1, p2};
}