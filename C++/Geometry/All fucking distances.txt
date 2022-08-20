//calculates distances between any pair of set {point, segment, ray, line}

const ld EPS = 1e-9;

template<typename T = ld>
struct pt {
    T x = 0, y = 0;

    pt() = default;

    pt(T a, T b): x(a), y(b) {}

    pt<T> operator+(const pt &p) const { return {x + p.x, y + p.y}; }

    pt<T> operator-(const pt &p) const { return {x - p.x, y - p.y}; }

    pt<T> operator*(const T c) const { return {x * c, y * c}; }

    pt<T> operator/(const T c) const { return {x / c, y / c}; }

    void operator+=(const pt &p) { x += p.x, y += p.y; }

    void operator-=(const pt &p) { x -= p.x, y -= p.y; }

    void operator*=(const T c) { x *= c, y *= c; }

    void operator/=(const T c) { x /= c, y /= c; }

    bool operator==(const pt &p) const {return p.x == x && p.y == y;}

    bool operator!=(const pt &p) const {return p.x != x || p.y != y;}

    ld dst(const pt &p) { return sqrtl((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)); }

    T sqdst(const pt &p) { return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y); }

    T dot(const pt &p) { return x * p.x + y * p.y; }

    T cross(const pt &p) { return x * p.y - y * p.x; }

    pt<ld> normalize(const ld EPS = 1e-9) {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        ld c = sqrtl(x * x + y * y);
        x /= c;
        y /= c;
        return pt(x, y);
    }

    bool operator<(const pt &p) const {return x < p.x || (x == p.x && y < p.y);}

    friend istream& operator>>(istream &is, pt &p) {
        is >> p.x >> p.y;
        return is;
    }

    friend ostream& operator<<(ostream &os, const pt &p) {
        return os << '{' << p.x << ", " << p.y << '}';
    }
};

template<typename T = ld>
struct line {
    T A, B, C;

    line(T AA, T BB, T CC) {
        A = AA;
        B = BB;
        C = CC;
        normalize();
    }

    line(pt<T> a, pt<T> b) {
        A = a.y - b.y;
        B = b.x - a.x;
        C = a.cross(b);
        normalize();
    }

    ld dst_to_pt(pt<T> &p) {
        normalize();
        return abs(A * p.x + B * p.y + C);
    }

    void normalize() {
        assert(abs(A) + abs(B) > EPS);
        ld u = sqrtl(A * A + B * B);
        A /= u, B /= u, C /= u;
        if (A < -EPS || (abs(A) < EPS && B < -EPS)) A *= -1, B *= -1, C *= -1;
    }

    line get_normal_from_point(pt<T> p) const {
        return line(p, p + pt(A, B));
    }

    void make_normal_from_point(pt<T> p) {
        pt p2 = p + pt(A, B);
        (*this) = line(p, p2);
    }

    pt<ld> intersect(const line<T> &l) const {
        assert(abs(A - l.A) + abs(B - l.B) > EPS);
        ld x = (l.C * B - C * l.B) / (A * l.B - l.A * B);
        ld y = (l.A * C - A * l.C) / (A * l.B - l.A * B);
        return {x, y};
    }

    bool is_parallel_to(line &l) const {
        return abs(l.A - A) + abs(l.B - B) < EPS;
    }

    bool is_equal_to(line &l) const {
        return abs(l.A - A) + abs(l.B - B) + abs(l.C - C) < EPS;
    }

    void printkxb() const {
        if (abs(B) < EPS) {
            cout << "X = " << -C / A << '\n';
        } else {
            cout << "Y = " << -A / B << "X + " << (-C / B) << '\n';
        }
    }

    void printABC() const {
        cout << A << " " << B << " " << C << '\n';
    }
};

ld dst_pp(pt<ld> p1, pt<ld> p2) {
    return p1.dst(p2);
}

int pir(pt<ld> p, pt<ld> p1, pt<ld> p2) {
    line l = line(p1, p2);
    if (l.dst_to_pt(p) > EPS) return 0;
    pt n1 = p2 - p1;
    pt n2 = p - p1;
    if (n1.dot(n2) >= -EPS) return 1;
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
    if (pis(i, p1, p2)) return dst_pp(p, i);
    return min(dst_pp(p, p1), dst_pp(p, p2));
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
    return dst_pp(p, p1);
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
    ll x1, y1, x2, y2, x3, y3, x4, y4;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
    pll m[] = {{x1, y1},
        {x2, y2},
        {x3, y3},
        {x4, y4}
    };
    assert(m[0] != m[1]);
    assert(m[2] != m[3]);
    pt<ld> a = {x1, y1};
    pt<ld> b = {x2, y2};
    pt<ld> c = {x3, y3};
    pt<ld> d = {x4, y4};
    cout << dst_pp(a, c) << "\n";
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