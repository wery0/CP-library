//Fast sorting by atan2, using cross product.
//Points with the same atan2 will be ordered by ascending distance from origin.
/*
Example:
    f({(1, 1), (0, -1), (-1, -1), (-1, 1), (1, -1), (-1, 0), (0, 0), (1, 0), (-2, -2), (0, 1)}) =
    = {(-1, -1), (-2, -2), (0, -1), (1, -1), (0, 0), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0)}
*/
//O(nlogn)
template<typename T>
void sort_points_by_atan2(vector<pair<T, T>>& m) {
    auto part = [](const pair<T, T>& p) -> int {
        if (p.second < 0) return 0;
        if (p.second == 0 && p.first >= 0) return 1;
        return 2;
    };
    sort(m.begin(), m.end(), [&](const auto& l, const auto& r) {
        int pl = part(l);
        int pr = part(r);
        if (pl != pr) return pl < pr;
        T cross = l.first * r.second - l.second * r.first;     //Ensure, that it fits in T
        if (cross) return cross > 0;
        return abs(l.first) + abs(l.second) < abs(r.first) + abs(r.second);
    });
}

//Specialization for point structure.
template<typename T>
void sort_points_by_atan2(vector<pt<T>>& m) {
    auto part = [](const pt<T>& p) -> int {
        if (p.y < 0) return 0;
        if (p.y == 0 && p.x >= 0) return 1;
        return 2;
    };
    sort(m.begin(), m.end(), [&](const auto& l, const auto& r) {
        int pl = part(l);
        int pr = part(r);
        if (pl != pr) return pl < pr;
        T cross = l.x * r.y - l.y * r.x;     //Ensure, that it fits in T
        if (cross) return cross > 0;
        return abs(l.x) + abs(l.y) < abs(r.x) + abs(r.y);
    });
}