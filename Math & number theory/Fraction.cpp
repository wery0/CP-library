struct fraction {
    ll num = 0;
    ll den = 1;

    void normalize() {
        if (den < 0) den *= -1, num *= -1;
        ll gc = __gcd(abs(num), den);
        num /= gc;
        den /= gc;
    }

    fraction() {}
    fraction(ll a): num(a) {}
    fraction(ll a, ll b): num(a), den(b) {normalize();}

    bool is_integer() const {return den == 1;}
    fraction operator-() const {return { -num, den};}

    fraction operator+(const fraction &other) const {return {num * other.den + other.num * den, den * other.den};}
    fraction operator-(const fraction &other) const {return {num * other.den - other.num * den, den * other.den};}
    fraction operator*(const fraction &other) const {return {num * other.num, den * other.den};}
    fraction operator/(const fraction &other) const {assert(other.num != 0); return {num * other.den, den * other.num};}

    void operator+=(const fraction &other) {
        num = num * other.den + other.num * den;
        den *= other.den;
        normalize();
    }

    void operator-=(const fraction &other) {
        num = num * other.den - other.num * den;
        den *= other.den;
        normalize();
    }

    void operator*=(const fraction &other) {
        num *= other.num;
        den *= other.den;
        normalize();
    }

    void operator/=(const fraction &other) {
        assert(other.num != 0);
        num *= other.den;
        den *= other.num;
        normalize();
    }

    bool operator!=(const fraction &other) const {return num * other.den != other.num * den;}
    bool operator==(const fraction &other) const {return num * other.den == other.num * den;}
    bool operator<(const fraction &other) const {return num * other.den < other.num * den;}
    bool operator>(const fraction &other) const {return num * other.den > other.num * den;}

    friend ostream &operator<<(ostream &os, const fraction &fr) {
        if (fr.den == 1) return os << fr.num;
        return os << "\\frac{" << fr.num << "}{" << fr.den << "}";
    }

    friend fraction inv(const fraction &f) {return {f.den, f.num};}
    friend fraction abs(const fraction &f) {return f > 0 ? f : -f;}
    friend int sign(const fraction &f) {return f.num < 0 ? -1 : f.num == 0 ? 0 : 1;}
};