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