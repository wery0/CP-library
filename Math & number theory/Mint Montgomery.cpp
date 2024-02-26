template<typename INT, typename LONG, typename std::make_unsigned<INT>::type mod>
class MontgomeryMint {
    static_assert(is_integral_v<INT> && is_integral_v<LONG>);
    static_assert(is_signed_v<INT> && is_signed_v<LONG>);
    static_assert(sizeof(INT) * 2 == sizeof(LONG));
    static_assert(mod & 1, "Modulo should be odd");
    static_assert(mod < numeric_limits<INT>::max() / 2 && "Modulo * 2 should fit into INT");

    using mint = MontgomeryMint;
    using UINT = typename std::make_unsigned<INT>::type;
    using ULONG = typename std::make_unsigned<LONG>::type;

    static constexpr UINT get_r() {
        UINT r = mod;
        for (int i = 0; i < __lg(sizeof(UINT)) + 2; ++i) {
            r *= UINT((INT)2 - ULONG(mod * r));
        }
        assert((UINT)(mod * r) == 1);
        return r;
    }

    static constexpr uint32_t reduce(const uint64_t& b) {
        return (b >> sh) + mod - ((ULONG(UINT(UINT(b) * r)) * mod) >> sh);
    }

    static constexpr UINT r = get_r();
    static constexpr UINT n2 = ULONG(-ULONG(mod)) % mod;  //auto y = -uintK_t((uintL_t)x), K, L in {8, 16} -> decltype(y) == int, LOL
    static constexpr int sh = sizeof(UINT) * 8;
    static_assert(UINT(r * mod) == 1, "This code has bugs :/");

    UINT a = 0;

public:
    using int_type = INT;
    using long_type = LONG;

    constexpr MontgomeryMint() = default;
    constexpr MontgomeryMint(const LONG& b): a(reduce(ULONG(b % mod + mod) * n2)) {};

    constexpr UINT get_val() const {UINT ret = reduce(a); return ret < mod ? ret : ret - mod;}
    static constexpr UINT get_mod() {return mod;}

    constexpr mint pow(ULONG pw) const {
        mint o(1);
        for (mint x(*this); pw; pw >>= 1) {
            if (pw & 1) o *= x;
            x *= x;
        }
        return o;
    }

    constexpr mint inv() const {
        INT x = get_val(), y = mod, u = 1, v = 0, t, tmp;
        while (y > 0) {
            t = x / y, x -= t * y, u -= t * v;
            tmp = x, x = y, y = tmp;
            tmp = u, u = v, v = tmp;
        }
        return mint{u};
    }

    friend MontgomeryMint pow(const MontgomeryMint& m, LONG pw) {return m.pow(pw);}
    friend MontgomeryMint inv(const MontgomeryMint& m) {return m.inv();}

    constexpr mint& operator+=(const mint& rhs) {if (INT(a += rhs.a - 2 * mod) < 0) a += 2 * mod; return *this;}
    constexpr mint& operator-=(const mint& rhs) {if (INT(a -= rhs.a) < 0) a += 2 * mod; return *this;}
    constexpr mint& operator*=(const mint& rhs) {a = reduce(ULONG(a) * rhs.a); return *this;}
    constexpr mint& operator/=(const mint& rhs) {*this *= rhs.inv(); return *this;}

    constexpr mint operator+(const mint& rhs) const {return mint(*this) += rhs;}
    constexpr mint operator-(const mint& rhs) const {return mint(*this) -= rhs;}
    constexpr mint operator*(const mint& rhs) const {return mint(*this) *= rhs;}
    constexpr mint operator/(const mint& rhs) const {return mint(*this) /= rhs;}
    constexpr mint operator-() const {return mint() - mint(*this);}

    static constexpr mint ONE = mint(1);
    mint& operator++() {*this += ONE; return *this;}
    mint operator++(int) {mint res = *this; ++*this; return res;}
    mint& operator--() {*this -= ONE; return *this;}
    mint operator--(int) {mint res = *this; ++*this; return res;}

    constexpr bool operator==(const mint& rhs) const {return (a >= mod ? a - mod : a) == (rhs.a >= mod ? rhs.a - mod : rhs.a);}
    constexpr bool operator!=(const mint& rhs) const {return (a >= mod ? a - mod : a) != (rhs.a >= mod ? rhs.a - mod : rhs.a);}
    constexpr bool operator<(const mint& rhs) const {return get_val() < rhs.get_val();}
    constexpr bool operator>(const mint& rhs) const {return get_val() > rhs.get_val();}

    explicit operator bool() const {return a != 0;}

    friend istream& operator>>(istream& is, mint& rhs) {LONG t; is >> t; rhs = mint(t); return is;}
    friend ostream& operator<<(ostream& os, const mint& rhs) {return os << rhs.get_val();}
};
using mint = MontgomeryMint<int32_t, int64_t, 998244353>;
