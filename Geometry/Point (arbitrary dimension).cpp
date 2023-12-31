//const long double EPS = 1e-9;
template<typename T, const size_t D, typename R = long double>
struct hpt {
    static_assert(D > 0);
    static_assert(is_floating_point_v<R>);

    array<T, D> p;

    hpt() {p.fill(0);}
    template<typename U>
    hpt(const array<U, D>& _p) {for (size_t i = 0; i < D; ++i) p[i] = _p[i];}
    template<typename U>
    hpt(const hpt<U, D>& _p) {for (size_t i = 0; i < D; ++i) p[i] = _p[i];}
    template<typename U>
    hpt(const initializer_list<U>& il) {
        assert(il.end() - il.begin() == D);
        auto it = il.begin();
        for (size_t i = 0; i < D; ++i, ++it) p[i] = *it;
    }

    size_t size() const {return D;}

    hpt<T, D> operator+(const hpt& p) const {auto res = *this; res += p; return res;}
    hpt<T, D> operator-(const hpt& p) const {auto res = *this; res -= p; return res;}
    hpt<T, D> operator*(const T c) const {auto res = *this; res *= c; return res;}
    hpt<T, D> operator/(const T c) const {auto res = *this; res /= c; return res;}
    hpt<T, D> operator-() const {auto res = *this; res *= -1; return res;}

    template<typename U>
    hpt<T, D>& operator+=(const hpt<U, D>& rhs) {for (size_t i = 0; i < D; ++i) p[i] += rhs.p[i]; return *this;}
    template<typename U>
    hpt<T, D>& operator-=(const hpt<U, D>& rhs) {for (size_t i = 0; i < D; ++i) p[i] -= rhs.p[i]; return *this;}
    hpt<T, D>& operator*=(const T c) {for (T& x : p) x *= c; return *this;}
    hpt<T, D>& operator/=(const T c) {for (T& x : p) x /= c; return *this;}

    template<typename U>
    auto dot(const hpt<U, D>& rhs) const {auto res = p[0] * rhs.p[0]; for (size_t i = 1; i < D; ++i) res += p[i] * rhs.p[i]; return res;}
    template<typename U>
    auto dist(const hpt<U, D>& rhs) const {auto res = (p[0] - rhs.p[0]) * (p[0] - rhs.p[0]); for (size_t i = 1; i < D; ++i) res += (p[i] - rhs.p[i]) * (p[i] - rhs.p[i]); return sqrtl((R)res);}
    template<typename U>
    R dist2(const hpt<U, D>& rhs) const {auto res = (p[0] - rhs.p[0]) * (p[0] - rhs.p[0]); for (size_t i = 1; i < D; ++i) res += (p[i] - rhs.p[i]) * (p[i] - rhs.p[i]); return res;}
    template<typename U>
    auto mdist(const hpt<U, D>& rhs) const {auto res = abs(p[0] - rhs.p[0]); for (size_t i = 1; i < D; ++i) res += abs(p[i] - rhs.p[i]); return res;}

    template<typename U>
    friend auto dot(const hpt& p1, const hpt<U, D>& p2) {return p1.dot(p2);}
    template<typename U>
    friend R dist(const hpt& p1, const hpt<U, D>& p2) {return p1.dist(p2);}
    template<typename U>
    friend auto dist2(const hpt& p1, const hpt<U, D>& p2) {return p1.dist2(p2);}
    template<typename U>
    friend auto mdist(const hpt& p1, const hpt<U, D>& p2) {return p1.mdist(p2);}

    auto begin() {return p.begin();}
    const auto begin() const {return p.begin();}
    auto end() {return p.end();}
    const auto end() const {return p.end();}

    template<typename U, const size_t DD>
    bool operator==(const hpt<U, DD>& rhs) const {
        if constexpr(is_integral_v<T> && is_integral_v<U>) return mdist(rhs) == 0;
        return mdist(rhs) < EPS;
    }
    template<typename U, const size_t DD>
    bool operator!=(const hpt<U, DD>& rhs) const {return !(*this == rhs);}
    template<typename U, const size_t DD>
    bool operator<(const hpt<U, DD>& rhs) const {
        static const auto eps = is_integral_v<T> && is_integral_v<U> ? 0 : EPS;
        for (size_t i = 0; i < D; ++i) {
            if (abs(p[0] - rhs[0]) > EPS) return p[0] < rhs[0];
        }
        return false;
    }

    R dist_to_origin() const {return sqrtl((R)dist2_to_origin());}
    T dist2_to_origin() const {T res = 0; for (const T x : p) res += x * x; return res;}

    void self_normalize() {
        T d2 = dist2_to_origin();
        assert(d2 > EPS && "Too close to 0!");
        R len = sqrtl(d2);
        for (T& x : p) x /= len;
    }

    hpt<R, D> get_normalized() const {
        hpt<R, D> res(*this);
        res.self_normalize();
        return res;
    }

    T& operator[](size_t i) {return p[i];}
    const T& operator[](size_t i) const {return p[i];}

    friend istream& operator>>(istream& is, hpt& p) {for (size_t i = 0; i < p.size(); ++i) is >> p[i]; return is;}
    friend ostream& operator<<(ostream& os, const hpt& p) {os << '{'; for (size_t i = 0; i < p.size(); ++i) os << p[i] << (i + 1 == p.size() ? "}" : ", "); return os;}
};
