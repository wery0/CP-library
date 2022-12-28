template<typename T = ld>
struct line {
    T A, B, C;

    line() = default;
    line(T A, T B, T C): A(A), B(B), C(C) {self_normalize();}
    template<typename U>
    line(const pt<U>& a, const pt<U>& b) {
        A = a.y - b.y;
        B = b.x - a.x;
        C = a.cross(b);
        self_normalize();
    }

    template<typename U>
    ld get_dst_to_pt(const pt<U>& p) const {
        if constexpr(is_integral_v<T>) {
            return abs(A * p.x + B * p.y + C) / sqrtl(A * A + B * B);
        } else {
            return abs(A * p.x + B * p.y + C);
        }
    }

    void self_normalize() {
        if constexpr(is_integral_v<T>) {
            assert(abs(A) + abs(B) > 0);
            T gc = __gcd(__gcd(abs(A), abs(B)), abs(C));
            A /= gc, B /= gc, C /= gc;
            if (A < 0 || (A == 0 && B < 0)) A *= -1, B *= -1, C *= -1;
        } else {
            assert(abs(A) + abs(B) > EPS);
            ld u = sqrtl(A * A + B * B);
            A /= u, B /= u, C /= u;
            if (A < -EPS || (abs(A) < EPS && B < -EPS)) A *= -1, B *= -1, C *= -1;
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
    pt<ld> get_projection_of_point(const pt<U>& p) const {
        ld dst = get_dst_to_pt(p);
        pt<ld> norm{A, B};
        norm.self_normalize();
        norm *= dst;
        pt<ld> o{(ld)p.x, (ld)p.y}; o += norm;
        if (get_dst_to_pt(o) < EPS) return o;
        o -= norm * 2;
        assert(get_dst_to_pt(o) < EPS);
        return o;
    }

    template<typename U>
    pt<ld> intersect(const line<U>& l) const {
        assert(abs(A - l.A) + abs(B - l.B) > EPS);
        ld x = (ld)1.0 * (l.C * B - C * l.B) / (A * l.B - l.A * B);
        ld y = (ld)1.0 * (l.A * C - A * l.C) / (A * l.B - l.A * B);
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
            cout << "X = " << -((ld)C / A) << '\n';
        } else {
            cout << "Y = " << -((ld)A / B) << "X + " << -((ld)C / B) << '\n';
        }
    }

    void printABC() const {
        cout << A << ' ' << B << ' ' << C << '\n';
    }
};