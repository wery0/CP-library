template<typename INT, typename LL, const INT MOD>
struct Mint {
    INT val = 0;

    Mint() = default;
    template<typename T>
    Mint(T c) {
        static_assert(is_integral<T>::value);
        if (c < 0) c += ((-c - 1) / MOD + 1) * MOD;
        else if (c >= MOD) c %= MOD;
        val = c;
    }

    template<typename I, typename L, const int M>
    friend Mint<I, L, M> binpow(const Mint<I, L, M>& m, ll pw) {
        L o = 1;
        for (L x = m.val; pw; pw >>= 1) {
            if (pw & 1) o = o * x % M;
            x = x * x % M;
        }
        return o;
    }

    template<typename I, typename L, const int M>
    friend Mint<I, L, M> inv(const Mint<I, L, M>& m) {
        return binpow(m, M - 2);
    }

    Mint operator+(const Mint& o) const {return val + o.val >= MOD ? val + o.val - MOD : val + o.val;}
    Mint operator-(const Mint& o) const {return val < o.val ? MOD - o.val + val : val - o.val;}
    Mint operator*(const Mint& o) const {return (LL)1 * val * o.val % MOD;}
    Mint operator/(const Mint& o) const {return (LL)1 * val * inv(o).val % MOD;}

    void operator+=(const Mint& o) {val += o.val; val -= val < MOD ? 0 : MOD;}
    void operator-=(const Mint& o) {val -= o.val; val += val < 0 ? MOD : 0;}
    void operator*=(const Mint& o) {val = (LL)1 * val * o.val % MOD;}
    void operator/=(const Mint& o) {val = (LL)1 * val * inv(o).val % MOD;}

    Mint& operator++() {val = val == MOD - 1 ? 0 : val + 1; return *this;}
    Mint operator++(int) {Mint res = *this; ++*this; return res;}

    bool operator==(const Mint& o) const {return val == o.val;}
    bool operator!=(const Mint& o) const {return val != o.val;}
    bool operator<(const Mint& o) const {return val < o.val;}
    bool operator>(const Mint& o) const {return val > o.val;}

    friend istream& operator>>(istream& in, Mint& m) {return in >> m.val;}
    friend ostream& operator<<(ostream& out, const Mint& m) {return out << m.val;}
    friend ostream& operator<<(ostream& out, const Mint&& m) {return out << m.val;}
};
using mint = Mint<int, ll, 998244353>;