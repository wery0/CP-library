//Use for constant modulo that is unknown in compile time
struct FastMod {
    uint64_t b, m;
    FastMod() = default;
    FastMod(uint64_t b): b(b), m(-1ULL / b) {}
    uint64_t operator()(uint64_t a) {  // a % b + (0 or b)
        return a - (uint64_t)((__uint128_t(m) * a) >> 64) * b;
    }
};
