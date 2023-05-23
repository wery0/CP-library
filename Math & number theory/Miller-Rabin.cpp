namespace MillerRabin {
    uint64_t binpow(uint64_t x, uint64_t k, const uint64_t mod) {
        uint64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = __uint128_t(o) * x % mod;
            x = __uint128_t(x) * x % mod;
        }
        return o;
    }

    bool is_prime(uint64_t p) {
        if (p < 2) return 0;
        static uint64_t A[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}; //enough for all <= 2^64 nums
        uint64_t k = __builtin_ctzll(p - 1), d = (p - 1) >> k;
        for (uint64_t a : A) {
            if (a >= p) break;
            uint64_t c = binpow(a, d, p);
            if (c == 1) continue;
            int fl = 0;
            for (uint64_t q = 0; q < k && !fl; ++q) {
                fl |= c == p - 1;
                c = __uint128_t(c) * c % p;
            }
            if (!fl) return 0;
        }
        return 1;
    }
};
