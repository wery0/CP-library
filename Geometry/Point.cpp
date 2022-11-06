template<typename T>
struct pt {
    T x = 0, y = 0;

    pt() = default;
    pt(T a, T b): x(a), y(b) {}

    pt<T> operator+(const pt &p) const { return {x + p.x, y + p.y}; }
    pt<T> operator-(const pt &p) const { return {x - p.x, y - p.y}; }
    pt<T> operator*(const T c) const { return {x * c, y * c}; }
    pt<T> operator/(const T c) const { return {x / c, y / c}; }
    pt<T> operator-() const {return { -x, -y};}

    void operator+=(const pt &p) { x += p.x, y += p.y; }
    void operator-=(const pt &p) { x -= p.x, y -= p.y; }
    void operator*=(const T c) { x *= c, y *= c; }
    void operator/=(const T c) { x /= c, y /= c; }

    bool operator==(const pt &p) const {return p.x == x && p.y == y;}
    bool operator!=(const pt &p) const {return p.x != x || p.y != y;}
    bool operator<(const pt &p) const {return x < p.x || (x == p.x && y < p.y);}

    T dot(const pt &p) const {return x * p.x + y * p.y;}
    T cross(const pt &p) const {return x * p.y - y * p.x;}
    ld dst(const pt &p) const {return sqrtl((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));}
    T sqdst(const pt &p) const {return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);}
    T mhdst(const pt &p) const {return abs(x - p.x) + abs(y - p.y);}

    friend T dot(const pt &p1, const pt &p2) {return p1.x * p2.x + p1.y * p2.y;}
    friend T cross(const pt &p1, const pt &p2) {return p1.x * p2.y - p1.y * p2.x;}
    friend ld dst(const pt &p1, const pt &p2) {return sqrtl((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));}
    friend T sqdst(const pt &p1, const pt &p2) {return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);}
    friend T mhdst(const pt &p1, const pt &p2) {return abs(p1.x - p2.x) + abs(p1.y - p2.y);}

    void self_rotate_about_origin(ld ang) {
        const ld sn = sinl(ang), cs = cosl(ang);
        ld nx = x * cs - sn * y;
        ld ny = x * sn + cs * y;
        x = nx, y = ny;
    }

    void self_normalize(const ld EPS = 1e-9) {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        ld c = sqrtl(x * x + y * y);
        x /= c;
        y /= c;
    }

    pt<ld> get_normalized(const ld EPS = 1e-9) const {
        if (abs(x) < EPS && abs(y) < EPS) assert(0);
        ld c = sqrtl(x * x + y * y);
        x /= c;
        y /= c;
        return pt(x, y);
    }

    friend istream& operator>>(istream &is, pt &p) {return is >> p.x >> p.y;}
    friend ostream& operator<<(ostream &os, const pt &p) {return os << '{' << p.x << ", " << p.y << '}';}
};