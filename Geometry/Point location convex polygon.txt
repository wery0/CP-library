template<typename T>
bool pis(pt<T> p1, pt<T> p2, pt<T> p) {
    return dot(p2 - p1, p - p1) >= 0 && dot(p1 - p2, p - p2) >= 0;
}

//1 - inside, 0 - border, -1 - outside
template<typename T>
int point_location_convex_polygon(vec<pt<T>> &ch, pt<T> &p) {
    int a = isz(ch);
    if (a == 0) return -1;
    if (a == 1) return -1; //could be 0 if p == ch[0]?
    if (a == 2) {
        return cross(ch[0] - p, ch[1] - p) == 0 && pis(ch[0], ch[1], p) ? 0 : -1;
    }
    if (p.x < ch[0].x) return -1;
    if (cross(p - ch[0], ch[1] - ch[0]) > 0) return -1;
    if (cross(ch.back() - ch[0], p - ch[0]) > 0) return -1;
    int l = 1, r = a;
    while (l + 1 < r) {
        int md = (l + r) / 2;
        pt<T> v = ch[md] - ch[0];
        if (cross(v, p - ch[0]) >= 0) l = md;
        else r = md;
    }
    pt<T> v = ch[l] - ch[0];
    if (cross(v, p - ch[0]) == 0) {
        return p - ch[0] == v ? 0 : pis(ch[0], ch[l], p) ? (l == 1 || l == a - 1 ? 0 : 1) : -1;
    }
    T val = cross(ch[l + 1] - p, p - ch[l]);
    return sign(val);
}