//Maintains minimum enclosing circle of 2D points.
//Expected O(1) per insert IF POINTS ARE ADDING IN RANDOM ORDER.
//T - type of points coordinates.
//D - floating point type you want to store answer in.
template<typename T, typename D = long double>
class minimum_enclosing_circle {
    static_assert(is_floating_point_v<D>);
    static constexpr D EPS = 1e-9;          //Change, if need

    template<typename K>
    struct pt {
        K x = 0, y = 0;

        pt() = default;
        pt(K a, K b): x(a), y(b) {}

        pt<K> operator+(const pt& p) const {return {x + p.x, y + p.y};}
        pt<K> operator/(const T c) const {return {x / c, y / c};}

        K dst2(const pt& p) const {return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);}
    };

    struct line {
        D A, B, C;

        line() = default;
        template<typename K>
        line(const pt<K>& a, const pt<K>& b) {
            A = a.y - b.y;
            B = b.x - a.x;
            C = a.x * b.y - a.y * b.x;
        }

        void make_normal_from_point(const pt<D>& p) {
            pt<D> p2 = p + pt{A, B};
            (*this) = line(p, p2);
        }

        pt<D> intersect(const line& l) const {
            assert(abs(A - l.A) + abs(B - l.B) > EPS);
            const D u = A * l.B - l.A * B;
            D x = (l.C * B - C * l.B) / u;
            D y = (l.A * C - A * l.C) / u;
            return {x, y};
        }
    };

    pt<D> c;
    D cr = 0;
    D cr2 = 0;
    vector<pt<T>> m;

    void solve1(int r, pt<T> p1, pt<T> p2) {
        for (int q = 0; q <= r; q++) {
            if ((c.x - m[q].x) * (c.x - m[q].x) + (c.y - m[q].y) * (c.y - m[q].y) < cr2 + EPS) continue;
            build_circle(p1, p2, m[q]);
        }
    }

    void solve2(int r, pt<T> p1) {
        for (int q = 0; q <= r; q++) {
            if ((c.x - m[q].x) * (c.x - m[q].x) + (c.y - m[q].y) * (c.y - m[q].y) < cr2 + EPS) continue;
            if (q == 0) {
                c = {(m[0].x + p1.x) / 2.0, (m[0].y + p1.y) / 2.0};
                cr2 = m[0].dst2(p1) / 4.0;
                cr = sqrtl(cr2);
                continue;
            }
            c = {(m[q].x + p1.x) / 2.0, (m[q].y + p1.y) / 2.0};
            cr2 = m[q].dst2(p1) / 4.0;
            cr = sqrtl(cr2);
            solve1(q - 1, m[q], p1);
        }
    }

    void build_circle(pt<T>& p1, pt<T>& p2, pt<T>& p3) {
        line l1 = line(p1, p2);
        line l2 = line(p2, p3);
        pt<D> m1 = {(p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0};
        pt<D> m2 = {(p2.x + p3.x) / 2.0, (p2.y + p3.y) / 2.0};
        l1.make_normal_from_point(m1);
        l2.make_normal_from_point(m2);
        c = l1.intersect(l2);
        cr2 = (c.x - p1.x) * (c.x - p1.x) + (c.y - p1.y) * (c.y - p1.y);
        cr = sqrtl(cr2);
    }

public:
    minimum_enclosing_circle() = default;

    D get_radius() const {return cr;}
    pair<D, D> get_center() const {return {c.x, c.y};}

    void insert(T x, T y) {
        static uniform_int_distribution<int> gen(0, INT32_MAX);
        static mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
        m.emplace_back(x, y);
        if (m.size() == 1) {
            c = {(D)x, (D)y};
        } else if (m.size() == 2) {
            c = {(m[0].x + m[1].x) / 2.0, (m[0].y + m[1].y) / 2.0};
            cr2 = m[0].dst2(m[1]) / 4.0;
            cr = sqrtl(cr2);
        } else {
            pt<T> u = {x, y};
            if ((c.x - u.x) * (c.x - u.x) + (c.y - u.y) * (c.y - u.y) < cr2 + EPS) return;
            c = {(D)x, (D)y};
            cr = 0;
            cr2 = 0;
            solve2(m.size() - 2, u);
        }
        if (gen(rnd) % m.size() == 0) {
            shuffle(m.begin(), m.end(), rnd);
        }
    }
};