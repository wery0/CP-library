template<typename INT, typename LONG, const INT MOD>
class Mint {
    static_assert(is_integral_v<INT> && is_integral_v<LONG>);
    static_assert(numeric_limits<INT>::max() < numeric_limits<LONG>::max() / numeric_limits<INT>::max() && "Product of two INT's must fit into LONG");

    INT val = 0;

public:
    using int_type = INT;
    using long_type = LONG;

    constexpr Mint() = default;
    template<typename T>
    constexpr Mint(T c) {
        static_assert(is_integral_v<T>);
        if (c < 0) c += ((-c - 1) / MOD + 1) * MOD;
        else if (c >= MOD) c %= MOD;
        val = c;
    }

    constexpr INT get_val() const {return val;}
    static constexpr INT get_mod() {return MOD;}

    Mint pow(LONG pw) const {
        LONG o = 1;
        for (LONG x = val; pw; pw >>= 1) {
            if (pw & 1) o = o * x % MOD;
            x = x * x % MOD;
        }
        return Mint(o);
    }

    Mint inv() const {
        return pow(MOD - 2);
    }

    friend Mint pow(const Mint& m, LONG pw) {return m.pow(pw);}
    friend Mint inv(const Mint& m) {return m.inv();}

    Mint operator+(const Mint& rhs) const {return val + rhs.val >= MOD ? val + rhs.val - MOD : val + rhs.val;}
    Mint operator-(const Mint& rhs) const {return val < rhs.val ? MOD - rhs.val + val : val - rhs.val;}
    Mint operator*(const Mint& rhs) const {return (LONG)1 * val * rhs.val % MOD;}
    Mint operator/(const Mint& rhs) const {return (LONG)1 * val * inv(rhs).val % MOD;}
    Mint operator-() const {return val ? MOD - val : val;}

    void operator+=(const Mint& rhs) {val += rhs.val; val -= val < MOD ? 0 : MOD;}
    void operator-=(const Mint& rhs) {val -= rhs.val; val += val < 0 ? MOD : 0;}
    void operator*=(const Mint& rhs) {val = (LONG)1 * val * rhs.val % MOD;}
    void operator/=(const Mint& rhs) {val = (LONG)1 * val * inv(rhs).val % MOD;}

    Mint& operator++() {val = val == MOD - 1 ? 0 : val + 1; return *this;}
    Mint operator++(int) {Mint res = *this; ++*this; return res;}
    Mint& operator--() {val = val ? val - 1 : MOD - 1; return *this;}
    Mint operator--(int) {Mint res = *this; --*this; return res;}

    bool operator==(const Mint& rhs) const {return val == rhs.val;}
    bool operator!=(const Mint& rhs) const {return val != rhs.val;}
    bool operator<(const Mint& rhs) const {return val < rhs.val;}
    bool operator>(const Mint& rhs) const {return val > rhs.val;}

    explicit operator bool() const {return val != 0;}

    friend istream& operator>>(istream& in, Mint& rhs) {return in >> rhs.val;}
    friend ostream& operator<<(ostream& out, const Mint& rhs) {return out << rhs.val;}
};
using mint = Mint<int32_t, int64_t, 998244353>;
