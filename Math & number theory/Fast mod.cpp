//Use for constant modulo that is unknown in compile time. Up to x5 speed up.
class fast_mod {
    uint64_t m, b;

public:
    fast_mod() = default;
    fast_mod(uint64_t m): m(m), b(-1ULL / m) {assert(m);}

    // x / m + (0 or 1)
    uint64_t quot(uint64_t x) {return (__uint128_t(b) * x) >> 64;}

    // x / m
    uint64_t honest_quot(uint64_t x) {uint64_t q = (__uint128_t(b) * x) >> 64; return x - q * m < m ? q : q + 1;}

    // x % m + (0 or m)
    uint64_t rem(uint64_t x) {return x - uint64_t((__uint128_t(b) * x) >> 64) * m;}

    // x % m
    uint64_t honest_rem(uint64_t x) {x -= uint64_t((__uint128_t(b) * x) >> 64) * m; return x < m ? x : x - m;}

    // {x / m + (0 or 1), x % m + (0 or m)}
    pair<uint64_t, uint64_t> quot_rem(uint64_t x) {  
        uint64_t q = (__uint128_t(b) * x) >> 64;
        return {q, x - q * m};
    }

    // {x / m, x % m}
    pair<uint64_t, uint64_t> honest_quot_rem(uint64_t x) {  
        uint64_t q = (__uint128_t(b) * x) >> 64;
        q += x - q * m < m ? 0 : 1;
        return {q, x - q * m};
    }
};
