//Use for constant modulo that is unknown in compile time. Up to x5 speed up.
template<typename T = uint64_t>
class fast_mod {
    using H =
    typename conditional<is_same_v<T, uint8_t>, uint16_t,
        typename conditional<is_same_v<T, uint16_t>, uint32_t,
            typename conditional<is_same_v<T, uint32_t>, uint64_t,
                typename conditional<is_same_v<T, uint64_t>, __uint128_t, int>::type
            >::type
        >::type
    >::type;

    static constexpr int szT = sizeof(T) * 8;
    static_assert(is_unsigned_v<T>);
    static_assert(is_unsigned_v<H> || is_same_v<H, __uint128_t>);
    static_assert(sizeof(H) * 8 == szT * 2);

    T m, b;

public:
    fast_mod() = default;
    constexpr fast_mod(T m): m(m), b(T(-1) / m) {assert(m);}

    // x / m + (0 or 1)
    T quot(T x) const {return (H(b) * x) >> szT;}

    // x / m
    T honest_quot(T x) const {T q = (H(b) * x) >> szT; return x - q * m < m ? q : q + 1;}

    // x % m + (0 or m)
    T rem(T x) const {return x - T((H(b) * x) >> szT) * m;}

    // x % m
    T honest_rem(T x) const {x -= T((H(b) * x) >> szT) * m; return x < m ? x : x - m;}

    // {x / m + (0 or 1), x % m + (0 or m)}
    __attribute__((always_inline))
    array<T, 2> quot_rem(T x) const {
        T q = (H(b) * x) >> szT;
        return {q, x - q * m};
    }

    // {x / m, x % m}
    __attribute__((always_inline))
    array<T, 2> honest_quot_rem(T x) const {
        T q = (H(b) * x) >> szT;
        x -= q * m;
        return x < m ? array<T, 2>{q, x} : array<T, 2>{q + 1, x - m};
    }
};
