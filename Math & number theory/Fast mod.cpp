//Use for constant modulo that is unknown in compile time. Up to x5 speed up.
template<typename T = uint64_t, typename H = __uint128_t>
class fast_mod {
    static constexpr int szT = sizeof(T) * 8;
    static constexpr int szH = sizeof(H) * 8;
    static_assert(is_unsigned_v<T>);
    static_assert(is_unsigned_v<H> || is_same_v<H, __uint128_t>);
    static_assert(szH == szT * 2);

    T m, b;

public:
    fast_mod() = default;
    fast_mod(T m): m(m), b(T(-1) / m) {assert(m);}

    // x / m + (0 or 1)
    T quot(T x) {return (H(b) * x) >> szT;}

    // x / m
    T honest_quot(T x) {T q = (H(b) * x) >> szT; return x - q * m < m ? q : q + 1;}

    // x % m + (0 or m)
    T rem(T x) {return x - T((H(b) * x) >> szT) * m;}

    // x % m
    T honest_rem(T x) {x -= T((H(b) * x) >> szT) * m; return x < m ? x : x - m;}

    // {x / m + (0 or 1), x % m + (0 or m)}
    pair<T, T> quot_rem(T x) {  
        T q = (H(b) * x) >> szT;
        return {q, x - q * m};
    }

    // {x / m, x % m}
    pair<T, T> honest_quot_rem(T x) {  
        T q = (H(b) * x) >> szT;
        q += x - q * m < m ? 0 : 1;
        return {q, x - q * m};
    }
};
