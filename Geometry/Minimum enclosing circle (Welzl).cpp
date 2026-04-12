//Implementaion of Welzl's algorithm for maintaining minimum enclosing circle of 2D points.
//Expected O(1) per insert IF POINTS ARE BEING ADDED IN RANDOM ORDER.
//T - type of points coordinates.
//D - floating point type you want to store answer in.
template<typename T, typename D = long double>
class welzl_2d {
    static_assert(is_floating_point_v<D>);
    static constexpr D EPS = 1e-6;             //Change if need

    template<typename K>
    struct pt {
        K x = 0, y = 0;
        pt() = default;
        template<typename U> pt(const pt<U>& p): x(p.x), y(p.y) {}
        pt(K a, K b): x(a), y(b) {}
        pt operator+(const pt& p) const {return {x + p.x, y + p.y};}
        pt operator-(const pt& p) const {return {x - p.x, y - p.y};}
        template<typename U> pt operator*(const U& c) const {return {x * c, y * c};}
        template<typename U> pt operator/(const U& c) const {return {x / c, y / c};}
        template<typename U> K dot(const pt<U>& p) const {return x * p.x + y * p.y;}
        template<typename U> K cross(const pt<U>& p) const {return x * p.y - y * p.x;}
        K norm2() const {return x * x + y * y;}
        template<typename U> auto dist2(const pt<U>& p) const {auto dx = x - p.x, dy = y - p.y; return dx * dx + dy * dy;}
    };

    struct circle {
        pt<D> c;
        D r2;
    };

    vector<pt<T>> m, n;
    circle cur = invalid_circle();

    circle invalid_circle() const {return {{0, 0}, -1};}

    bool contains(const circle& s, const pt<T>& p) const {return 0 <= s.r2 && s.c.dist2(p) <= s.r2 + EPS;}

    circle circle_from(const vector<pt<T>>& R) const {
        if (R.empty()) return invalid_circle();
        if (R.size() == 1) return {pt<D>(R[0]), 0};
        if (R.size() == 2) {
            pt<D> c = pt<D>(R[0] + R[1]) / 2;
            return {c, c.dist2(R[0])};
        }
        assert(R.size() == 3);
        pt<D> u = R[1] - R[0], v = R[2] - R[0];
        D det = u.cross(v) * 2;
        if (abs(det) <= EPS) return invalid_circle();
        pt<D> sh = pt<D>(-u.y, u.x) * v.norm2() - pt<D>(-v.y, v.x) * u.norm2();
        pt<D> c = pt<D>(R[0]) + sh / det;
        return {c, c.dist2(R[0])};
    }

    circle calc_mec(const vector<pt<T>>& R) const {
        return circle_from(R);
    }

    void set_support(const vector<pt<T>>& R) {
        n = R;
        cur = calc_mec(n);
    }

public:
    welzl_2d(size_t cnt = 0) {m.reserve(cnt);}

    void clear() {m.clear(); n.clear(); cur = invalid_circle();}

    pair<pt<D>, D> get_current_mec() const {
        assert(n.size());
        return {cur.c, sqrtl(cur.r2)};
    }

    void insert(T x, T y) {
        m.emplace_back(x, y);
        if (contains(cur, m.back())) return;
        set_support({m.back()});
        for (int i = 0; i < m.size() - 1; ++i) {
            if (contains(cur, m[i])) continue;
            set_support({m.back(), m[i]});
            for (int j = 0; j < i; ++j) {
                if (!contains(cur, m[j])) set_support({m.back(), m[i], m[j]});
            }
        }
    }
};
