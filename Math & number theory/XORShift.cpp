template<typename T = int64_t>
class xor_shift {
    T seed;

public:
    xor_shift(T seed = -1): seed(seed == -1 ? chrono::steady_clock::now().time_since_epoch().count() : seed) {}

    T gen() {
        seed ^= seed << 21;
        seed ^= seed >> 35;
        seed ^= seed << 4;
        return seed;
    }

    T gen_mod(T mod) {
        return abs(gen() % mod);
    }

    T gen_in_range(T l, T r) {
        return l + gen_mod(r - l + 1);
    }
};
