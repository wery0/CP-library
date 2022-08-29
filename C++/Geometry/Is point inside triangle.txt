template<typename T>
T double_triangle_surface(pt<T> p1, pt<T> p2, pt<T> p3) {
    return abs(cross(p2 - p1, p3 - p1));
}

template<typename T>
bool is_point_inside_triangle(pt<T> p1, pt<T> p2, pt<T> p3, pt<T> p) {
    T s = double_triangle_surface(p1, p2, p3);
    T s1 = double_triangle_surface(p, p2, p3);
    T s2 = double_triangle_surface(p1, p, p3);
    T s3 = double_triangle_surface(p1, p2, p);
    return s == s1 + s2 + s3;
}