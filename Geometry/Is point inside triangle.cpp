//Checks if point p is inside or on the border of triangle {p1, p2, p3}
template<typename T>
bool is_point_inside_triangle(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p) {
    T s = abs(cross(p2 - p1, p3 - p1));
    T s1 = abs(cross(p1 - p, p2 - p));
    T s2 = abs(cross(p2 - p, p3 - p));
    T s3 = abs(cross(p3 - p, p1 - p));
    if (s == 0 && s1 + s2 + s3 == 0) {
        T d1 = dot(p1 - p, p2 - p);
        T d2 = dot(p2 - p, p3 - p);
        T d3 = dot(p3 - p, p1 - p);
        return min({d1, d2, d3}) <= 0 && max({d1, d2, d3}) >= 0;
    }
    return s == s1 + s2 + s3;
}
