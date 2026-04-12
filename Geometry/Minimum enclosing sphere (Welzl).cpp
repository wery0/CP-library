//Implementaion of Welzl's algorithm for maintaining minimum enclosing sphere of 3D points.
//Expected O(1) per insert IF POINTS ARE BEING ADDED IN RANDOM ORDER.
//T - type of points coordinates.
//D - floating point type you want to store answer in.
template<typename T, typename D = long double>
class welzl_3d {
    static_assert(is_floating_point_v<D>);
    static constexpr D EPS = 1e-3;             //Change if need

    template<typename K>
    struct pt {
        K x = 0, y = 0, z = 0;
        pt() = default;
        template<typename U> pt(const pt<U>& p): x(p.x), y(p.y), z(p.z) {}
        pt(K a, K b, K c): x(a), y(b), z(c) {}
        pt operator+(const pt& p) const {return {x + p.x, y + p.y, z + p.z};}
        pt operator-(const pt& p) const {return {x - p.x, y - p.y, z - p.z};}
        template<typename U> pt operator*(const U& c) const {return {x * c, y * c, z * c};}
        template<typename U> pt operator/(const U& c) const {return {x / c, y / c, z / c};}
        template<typename U> K dot(const pt<U>& p) const {return x * p.x + y * p.y + z * p.z;}
        template<typename U> pt<K> cross(const pt<U>& p) const {return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x};}
        K norm2() const {return x * x + y * y + z * z;}
        template<typename U> auto dist2(const pt<U>& p) const {auto dx = x - p.x, dy = y - p.y, dz = z - p.z; return dx * dx + dy * dy + dz * dz;}
    };

    struct sphere {
        pt<D> c;
        D r2;
    };

    vector<pt<T>> m, n;
    sphere cur = invalid_sphere();

    sphere invalid_sphere() const {return {{0, 0, 0}, -1};}

    bool contains(const sphere& s, const pt<T>& p) const {return 0 <= s.r2 && s.c.dist2(p) <= s.r2 + EPS;}

    sphere sphere_from(const vector<pt<T>>& R) const {
        if (R.empty()) return invalid_sphere();
        if (R.size() == 1) return {pt<D>(R[0]), 0};
        if (R.size() == 2) {
            pt<D> c = pt<D>(R[0] + R[1]) / 2;
            return {c, c.dist2(R[0])};
        }
        if (R.size() == 3) {
            pt<D> u = R[1] - R[0], v = R[2] - R[0];
            pt<D> w = u.cross(v);
            D wn2 = w.norm2() * 2;
            if (wn2 <= EPS) return invalid_sphere();
            pt<D> sh = (v.cross(w) * u.norm2() + w.cross(u) * v.norm2());
            pt<D> c = pt<D>(R[0]) + sh / wn2;
            return {c, c.dist2(R[0])};
        }
        assert(R.size() == 4);
        pt<D> u = R[1] - R[0], v = R[2] - R[0], w = R[3] - R[0];
        D det = u.dot(v.cross(w)) * 2;
        if (abs(det) <= EPS) return invalid_sphere();
        pt<D> sh = v.cross(w) * u.norm2() + w.cross(u) * v.norm2() + u.cross(v) * w.norm2();
        pt<D> c = pt<D>(R[0]) + sh / det;
        return {c, c.dist2(R[0])};
    }

    sphere calc_mes(const vector<pt<T>>& R) const {
        int sz = R.size();
        if (sz <= 2) return sphere_from(R);
        sphere best = invalid_sphere();
        auto relax = [&](const vector<pt<T>>& S) {
            sphere s = sphere_from(S);
            if (s.r2 < 0 || any_of(R.begin(), R.end(), [&](const pt<T>& p) {return !contains(s, p);})) return;
            if (best.r2 < 0 || s.r2 < best.r2) best = s;
        };
        if (sz == 3) {
            relax({R[0], R[1]});
            relax({R[0], R[2]});
            relax({R[1], R[2]});
            relax(R);
            return best;
        }
        relax({R[0], R[1]});
        relax({R[0], R[2]});
        relax({R[0], R[3]});
        relax({R[1], R[2]});
        relax({R[1], R[3]});
        relax({R[2], R[3]});
        relax({R[0], R[1], R[2]});
        relax({R[0], R[1], R[3]});
        relax({R[0], R[2], R[3]});
        relax({R[1], R[2], R[3]});
        relax(R);
        return best;
    }

    void set_support(const vector<pt<T>>& R) {
        n = R;
        cur = calc_mes(n);
    }

public:
    welzl_3d(size_t cnt = 0) {m.reserve(cnt);}

    void clear() {m.clear(); n.clear(); cur = invalid_sphere();}

    pair<pt<D>, D> get_current_mes() const {
        assert(n.size());
        return {cur.c, sqrtl(cur.r2)};
    }

    void insert(T x, T y, T z) {
        m.emplace_back(x, y, z);
        if (contains(cur, m.back())) return;
        set_support({m.back()});
        for (int i = 0; i < m.size() - 1; ++i) {
            if (contains(cur, m[i])) continue;
            set_support({m.back(), m[i]});
            for (int j = 0; j < i; ++j) {
                if (contains(cur, m[j])) continue;
                set_support({m.back(), m[i], m[j]});
                for (int k = 0; k < j; ++k) {
                    if (!contains(cur, m[k])) set_support({m.back(), m[i], m[j], m[k]});
                }
            }
        }
    }
};
