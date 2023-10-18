//These functions calculate distance between any pair of {point, segment, ray, line}

template<typename T, typename T1, typename T2>
bool is_point_on_ray(const pt<T>& p, const pt<T1>& p1, const pt<T2>& p2) {
    return abs(cross(p - p1, p - p2)) <= EPS && dot(p2 - p1, p - p1) >= -EPS;
}

template<typename T, typename T1, typename T2>
bool is_point_on_segment(const pt<T>& p, const pt<T1>& p1, const pt<T2>& p2) {
    return abs(cross(p - p1, p - p2)) <= EPS && dot(p2 - p1, p - p1) >= -EPS && dot(p1 - p2, p - p2) >= -EPS;
}

template<typename T, typename D = long double>
D dist_point_segment(const pt<T>& p, const pt<T>& p1, const pt<T>& p2) {
    line l = line(p1, p2);
    pt i = l.get_projection_of_point(p);
    return is_point_on_segment(i, p1, p2) ? dist(p, i) : min(dist(p, p1), dist(p, p2));
}

template<typename T, typename D = long double>
D dist_point_line(const pt<T>& p, const pt<T>& p1, const pt<T>& p2) {
    return line(p1, p2).get_dist_to_pt(p);
}

template<typename T, typename D = long double>
D dist_segment_segment(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    D dst = min({
        dist_point_segment(p1, p3, p4), dist_point_segment(p2, p3, p4),
        dist_point_segment(p3, p1, p2), dist_point_segment(p4, p1, p2)
    });
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt i = l1.intersect(l2);
    return is_point_on_segment(i, p1, p2) && is_point_on_segment(i, p3, p4) ? 0 : dst;
}

template<typename T, typename D = long double>
D dist_point_ray(const pt<T>& p, const pt<T>& p1, const pt<T>& p2) {
    line l = line(p1, p2);
    pt i = l.get_projection_of_point(p);
    return is_point_on_ray(i, p1, p2) ? dist_point_line(p, p1, p2) : dist(p, p1);
}

template<typename T, typename D = long double>
D dist_segment_ray(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    D dst = min({
        dist_point_ray(p1, p3, p4),
        dist_point_ray(p2, p3, p4),
        dist_point_segment(p3, p1, p2)
    });
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt i = l1.intersect(l2);
    return is_point_on_segment(i, p1, p2) && is_point_on_ray(i, p3, p4) ? 0 : dst;
}

template<typename T, typename D = long double>
D dist_segment_line(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    return min(dist_segment_ray(p1, p2, p3, p4), dist_segment_ray(p1, p2, p4, p3));
}

template<typename T, typename D = long double>
D dist_ray_ray(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    D dst = min(dist_point_ray(p1, p3, p4), dist_point_ray(p3, p1, p2));
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt i = l1.intersect(l2);
    return is_point_on_ray(i, p1, p2) && is_point_on_ray(i, p3, p4) ? 0 : dst;
}

template<typename T, typename D = long double>
D dist_ray_line(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    return min(dist_ray_ray(p1, p2, p3, p4), dist_ray_ray(p1, p2, p4, p3));
}

template<typename T, typename D = long double>
D dist_line_line(const pt<T>& p1, const pt<T>& p2, const pt<T>& p3, const pt<T>& p4) {
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    return l1.is_parallel_to(l2) ? dist_point_line(p1, p3, p4) : 0;
}
