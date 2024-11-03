//Fast sorting by atan2, using cross product.
//Points with the same atan2 will be ordered by ascending distance from origin.
/*
Example:
    f({(1, 1), (0, -1), (-1, -1), (-1, 1), (1, -1), (-1, 0), (0, 0), (1, 0), (-2, -2), (0, 1)}) =
    = {(-1, -1), (-2, -2), (0, -1), (1, -1), (0, 0), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0)}
*/
//O(nlogn)
template<typename T>
void sort_points_by_atan2(vector<pt<T>>& m) {
    const T eps = is_integral_v<T> ? 0 : EPS;
    auto part = [&](const pt<T>& p) -> int {
        return p.y < -eps ? 0 : abs(p.y) <= eps && p.x >= -eps ? 1 : 2;
    };
    sort(m.begin(), m.end(), [&](const pt<T>& l, const pt<T>& r) {
        int pl = part(l), pr = part(r);
        if (pl != pr) return pl < pr;
        T cross = l.x * r.y - l.y * r.x;     //Ensure, that it fits in T
        if (abs(cross) > eps) return cross > 0;
        return abs(l.x) + abs(l.y) < abs(r.x) + abs(r.y);
    });
}

//Specialization for pair
template<typename T>
void sort_points_by_atan2(vector<pair<T, T>>& m) {
    const T eps = is_integral_v<T> ? 0 : EPS;
    auto part = [&](const pair<T, T>& p) -> int {
        return p.second < -eps ? 0 : abs(p.second) <= eps && p.first >= -eps ? 1 : 2;
    };
    sort(m.begin(), m.end(), [&](const pair<T, T>& l, const pair<T, T>& r) {
        int pl = part(l), pr = part(r);
        if (pl != pr) return pl < pr;
        T cross = l.first * r.second - l.second * r.first;     //Ensure, that it fits in T
        if (abs(cross) > eps) return cross > 0;
        return abs(l.first) + abs(l.second) < abs(r.first) + abs(r.second);
    });
}
