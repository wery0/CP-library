template<typename T = int64_t>
class Fraction {
    static_assert(is_integral_v<T> && is_signed_v<T>, "Numeric type must be signed and integral");

    T numerator_ = 0;
    T denominator_ = 1;

    static T binpow(T val, T pow) {
        T res = 1;
        for (T x = val; pow; pow >>= 1) {
            if (pow & 1) res *= x;
            x *= x;
        }
        return res;
    }

    void normalize() {
        assert(denominator_ != 0);
        if (denominator_ < 0) {
            denominator_ *= -1;
            numerator_ *= -1;
        }
        T gcd = __gcd(abs(numerator_), denominator_);
        numerator_ /= gcd;
        denominator_ /= gcd;
    }

public:
    Fraction() = default;

    template<typename U>
    Fraction(U value) : numerator_(value) {
        static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
    }

    template <typename U>
    Fraction(U numerator, U denominator) : numerator_(numerator), denominator_(denominator) {
        static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
        assert(denominator_ && "Fraction can't have a zero denominator");
        normalize();
    }

    Fraction& operator+=(const Fraction& rhs) {
        numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res += rhs;
        return res;
    }

    Fraction& operator-=(const Fraction& rhs) {
        numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res -= rhs;
        return res;
    }

    Fraction& operator*=(const Fraction& rhs) {
        numerator_ *= rhs.numerator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res *= rhs;
        return res;
    }

    Fraction& operator/=(const Fraction& rhs) {
        assert(rhs.numerator_ && "Division by zero!");
        numerator_ *= rhs.denominator_;
        denominator_ *= rhs.numerator_;
        normalize();
        return *this;
    }
    friend Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res /= rhs;
        return res;
    }

    Fraction operator-() const { return {-numerator_, denominator_}; }

    bool operator==(const Fraction& rhs) const { return numerator_ == rhs.numerator_ && denominator_ == rhs.denominator_; }
    friend bool operator!=(const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
    bool operator<(const Fraction& rhs) const { return numerator_ * rhs.denominator_ < rhs.numerator_ * denominator_; }
    friend bool operator>(const Fraction& lhs, const Fraction& rhs) { return rhs < lhs; }
    friend bool operator<=(const Fraction& lhs, const Fraction& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Fraction& lhs, const Fraction& rhs) { return !(lhs < rhs); }

    void pow(T power) {
        if (numerator_ == 0) return;
        T sign = numerator_ > 0 ? 1 : -1;
        numerator_ *= sign;
        if (power < 0) {
            swap(numerator_, denominator_);
            power *= -1;
        }
        numerator_ = binpow(numerator_, power);
        denominator_ = binpow(denominator_, power);
        if ((power & 1) && sign == -1) numerator_ *= -1;
    }
    friend Fraction pow(const Fraction& rhs, T power) {
        Fraction res = rhs;
        res.pow(power);
        return res;
    }

    void invert() {
        assert(numerator_ != 0 && "Can't invert 0");
        swap(numerator_, denominator_);
    }
    friend Fraction invert(const Fraction& rhs) {
        Fraction res = rhs;
        res.invert();
        return res;
    }

    bool is_zero() const { return numerator_ == 0; }
    bool is_integer() const { return denominator_ == 1; }
    int get_sign() const { return numerator_ < 0 ? -1 : numerator_ == 0 ? 0 : 1; }
    T get_numerator() const { return numerator_; }
    T get_denominator() const { return denominator_; }
    friend Fraction abs(const Fraction& rhs) { return rhs.numerator_ > 0 ? rhs : -rhs; }

    friend ostream& operator<<(ostream& os, const Fraction& rhs) {
        if (rhs.is_integer()) return os << rhs.numerator_;
        return os << string("\\frac{") << rhs.numerator_ << "}{" << rhs.denominator_ << "}";
    }
};