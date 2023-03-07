const long double EPS = 1e-9;

//T - type for coordinates
//D - floating point type for noninteger calculations (sqrt for example)
template<typename T, typename D = long double>
struct pt {
    static_assert(is_floating_point_v<D>);
    T x = 0, y = 0;

    pt() = default;
    template<typename U>
    pt(U a, U b): x(a), y(b) {}
    template<typename U>
    pt(const pt<U>& p): x(p.x), y(p.y) {}

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

    void self_normalize() {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        D c = hypotl(x, y);
        x /= c, y /= c;
    }

    pt<D> get_normalized() const {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        D c = hypotl(x, y);
        return pt(x / c, y / c);
    }

    friend istream& operator>>(istream& is, pt& p) {return is >> p.x >> p.y;}
    friend ostream& operator<<(ostream& os, const pt& p) {return os << '{' << p.x << ", " << p.y << '}';}
};