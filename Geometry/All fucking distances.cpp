//Calculates distance between any pair of {point, segment, ray, line}

const ld EPS = 1e-9;
//T - type for coordinates
//D - floating point type for noninteger calculations (sqrt for example)
template<typename T, typename D = long double>
struct pt {
    static_assert(is_floating_point_v<D>);
    T x = 0, y = 0;

    pt() = default;
    pt(T a, T b): x(a), y(b) {}

    pt<T> operator+(const pt& p) const {return {x + p.x, y + p.y};}
    pt<T> operator-(const pt& p) const {return {x - p.x, y - p.y};}
    pt<T> operator*(const T c) const {return {x * c, y * c};}
    pt<T> operator/(const T c) const {return {x / c, y / c};}
    pt<T> operator-() const {return {-x, -y};}

    void operator+=(const pt& p) {x += p.x, y += p.y;}
    void operator-=(const pt& p) {x -= p.x, y -= p.y;}
    void operator*=(const T c) {x *= c, y *= c;}
    void operator/=(const T c) {x /= c, y /= c;}

    bool operator==(const pt& p) const {return p.x == x && p.y == y;}
    bool operator!=(const pt& p) const {return p.x != x || p.y != y;}
    bool operator<(const pt& p) const {return x < p.x || (x == p.x && y < p.y);}

    T dot(const pt& p) const {return x * p.x + y * p.y;}
    T cross(const pt& p) const {return x * p.y - y * p.x;}
    D dist(const pt& p) const {return hypotl(x - p.x, y - p.y);}
    T dist2(const pt& p) const {return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);}
    T mdist(const pt& p) const {return abs(x - p.x) + abs(y - p.y);}

    friend T dot(const pt& p1, const pt& p2) {return p1.x * p2.x + p1.y * p2.y;}
    friend T cross(const pt& p1, const pt& p2) {return p1.x * p2.y - p1.y * p2.x;}
    friend D dist(const pt& p1, const pt& p2) {return hypotl(p1.x - p2.x, p1.y - p2.y);}
    friend T dist2(const pt& p1, const pt& p2) {return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);}
    friend T mdist(const pt& p1, const pt& p2) {return abs(p1.x - p2.x) + abs(p1.y - p2.y);}

    void self_rotate_about_origin(D ang) {
        const D sn = sinl(ang), cs = cosl(ang);
        D nx = x * cs - sn * y;
        D ny = x * sn + cs * y;
        x = nx, y = ny;
    }

    void self_normalize(const D EPS = 1e-9) {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        D c = hypotl(x, y);
        x /= c, y /= c;
    }

    pt<D> get_normalized(const D EPS = 1e-9) const {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        D c = hypotl(x, y);
        x /= c, y /= c;
        return pt(x, y);
    }

    friend istream& operator>>(istream& is, pt& p) {return is >> p.x >> p.y;}
    friend ostream& operator<<(ostream& os, const pt& p) {return os << '{' << p.x << ", " << p.y << '}';}
};

//T - type of A, B, C coefficients
template<typename T = long double, typename D = long double>
struct line {
    static_assert(is_floating_point_v<D>);
    T A, B, C;

    line() = default;
    line(T A, T B, T C): A(A), B(B), C(C) {self_normalize();}
    template<typename U>
    line(const pt<U>& a, const pt<U>& b) {
        A = a.y - b.y;
        B = b.x - a.x;
        C = cross(a, b);
        self_normalize();
    }

    template<typename U>
    D get_dist_to_pt(const pt<U>& p) const {
        if constexpr(is_integral_v<T>) {
            return abs(A * p.x + B * p.y + C) / hypotl(A, B);
        } else {
            return abs(A * p.x + B * p.y + C);
        }
    }

    void self_normalize() {
        if constexpr(is_integral_v<T>) {
            assert(abs(A) + abs(B) > 0);
            T gc = __gcd(__gcd(abs(A), abs(B)), abs(C));
            if (A < 0 || (A == 0 && B < 0)) gc *= -1;
            A /= gc, B /= gc, C /= gc;
        } else {
            assert(abs(A) + abs(B) > EPS);
            D u = hypotl(A, B);
            if (A < -EPS || (abs(A) < EPS && B < -EPS)) u *= -1;
            A /= u, B /= u, C /= u;
        }
    }

    template<typename U>
    line get_normal_from_point(const pt<U>& p) const {
        return line(p, p + pt(A, B));
    }

    template<typename U>
    void make_normal_from_point(const pt<U>& p) {
        pt p2 = p + pt{A, B};
        (*this) = line(p, p2);
    }

    template<typename U>
    pt<D> get_projection_of_point(const pt<U>& p) const {
        D dst = get_dist_to_pt(p);
        pt<D> norm{A, B};
        norm.self_normalize();
        norm *= dst;
        pt<D> o{(D)p.x, (D)p.y}; o += norm;
        if (get_dist_to_pt(o) < EPS) return o;
        o -= norm * 2;
        assert(get_dist_to_pt(o) < EPS);
        return o;
    }

    template<typename U>
    pt<D> intersect(const line<U>& l) const {
        assert(abs(A - l.A) + abs(B - l.B) > EPS);
        D x = (D)1.0 * (l.C * B - C * l.B) / (A * l.B - l.A * B);
        D y = (D)1.0 * (l.A * C - A * l.C) / (A * l.B - l.A * B);
        return {x, y};
    }

    template<typename U>
    bool is_parallel_to(const line<U>& l) const {
        return abs(l.A - A) + abs(l.B - B) < EPS;
    }

    template<typename U>
    bool is_equal_to(const line<U>& l) const {
        return abs(l.A - A) + abs(l.B - B) + abs(l.C - C) < EPS;
    }

    void printkxb() const {
        if (abs(B) < EPS) {
            cout << "X = " << -((D)C / A) << '\n';
        } else {
            cout << "Y = " << -((D)A / B) << "X + " << -((D)C / B) << '\n';
        }
    }

    void printABC() const {
        cout << A << ' ' << B << ' ' << C << '\n';
    }
};

int pir(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    line l = line(p1, p2);
    if (l.get_dist_to_pt(p) > EPS) return 0;
    pt n1 = p2 - p1;
    pt n2 = p - p1;
    if (dot(n1, n2) >= -EPS) return 1;
    return 0;
}

int pis(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    return pir(p, p1, p2) && pir(p, p2, p1);
}

ld dst_ps(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    line l = line(p1, p2);
    line u = line(p1, p2);
    u.make_normal_from_point(p);
    pt i = l.intersect(u);
    if (pis(i, p1, p2)) return dist(p, i);
    return min(dist(p, p1), dist(p, p2));
}

ld dst_pl(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    line l = line(p1, p2);
    return abs(l.A * p.x + l.B * p.y + l.C);
}

ld dst_ss(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld dst = min({dst_ps(p1, p3, p4), dst_ps(p2, p3, p4),
                  dst_ps(p3, p1, p2), dst_ps(p4, p1, p2)
                 });
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt i = l1.intersect(l2);
    if (pis(i, p1, p2) && pis(i, p3, p4)) return 0;
    return dst;
}

ld dst_pr(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    line l = line(p1, p2);
    line i = line(p1, p2);
    i.make_normal_from_point(p);
    pt c = l.intersect(i);
    if (pir(c, p1, p2)) return dst_pl(p, p1, p2);
    return dist(p, p1);
}

ld dst_sr(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld dst = min(dst_pr(p1, p3, p4), dst_pr(p2, p3, p4));
    dst = min(dst, dst_ps(p3, p1, p2));
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt<ld> p = l1.intersect(l2);
    if (pis(p, p1, p2) && pir(p, p3, p4)) return 0;
    return dst;
}

ld dst_sl(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld d1 = dst_sr(p1, p2, p3, p4);
    ld d2 = dst_sr(p1, p2, p3, p3 - (p4 - p3));
    return min(d1, d2);
}

ld dst_rr(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld dst = min(dst_pr(p1, p3, p4), dst_pr(p3, p1, p2));
    line l1 = line(p1, p2);
    line l2 = line(p3, p4);
    if (l1.is_parallel_to(l2)) return dst;
    pt<ld> p = l1.intersect(l2);
    if (pir(p, p1, p2) && pir(p, p3, p4)) return 0;
    return dst;
}

ld dst_rl(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld d1 = dst_rr(p1, p2, p3, p4);
    ld d2 = dst_rr(p1, p2, p3, p3 - (p4 - p3));
    return min(d1, d2);
}

ld dst_ll(pt<ld> p1, pt<ld> p2, pt<ld> p3, pt<ld> p4) {
    ld d1 = dst_rl(p1, p2, p3, p4);
    ld d2 = dst_rl(p1, p1 - (p2 - p1), p3, p4);
    return min(d1, d2);
}

int main() {
    fast;
    cout.precision(12);
    cout << fixed;
    ld x1, y1, x2, y2, x3, y3, x4, y4;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
    pt<ld> a = {x1, y1};
    pt<ld> b = {x2, y2};
    pt<ld> c = {x3, y3};
    pt<ld> d = {x4, y4};
    cout << dist(a, c) << "\n";
    cout << dst_ps(a, c, d) << "\n";
    cout << dst_pr(a, c, d) << "\n";
    cout << dst_pl(a, c, d) << "\n";
    // cout << "\n";
    cout << dst_ps(c, a, b) << "\n";
    cout << dst_ss(a, b, c, d) << "\n";
    cout << dst_sr(a, b, c, d) << "\n";
    cout << dst_sl(a, b, c, d) << "\n";
    // cout << "\n";
    cout << dst_pr(c, a, b) << "\n";
    cout << dst_sr(c, d, a, b) << "\n";
    cout << dst_rr(a, b, c, d) << "\n";
    cout << dst_rl(a, b, c, d) << "\n";
    // cout << "\n";
    cout << dst_pl(c, a, b) << "\n";
    cout << dst_sl(c, d, a, b) << "\n";
    cout << dst_rl(c, d, a, b) << "\n";
    cout << dst_ll(a, b, c, d) << "\n";
}