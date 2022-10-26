namespace MillerRabin {
    inline ull binpow(ull x, ull k, const ull mod) {
        ull o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = __uint128_t(o) * x % mod;
            x = __uint128_t(x) * x % mod;
        }
        return o;
    }

    bool is_prime(ull p) {
        if (p < 2) return 0;
        static ull A[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}; //enough for all <= 2^64 nums
        ull k = __builtin_ctzll(p - 1), d = (p - 1) >> k;
        for (ull a : A) {
            if (a >= p) break;
            ull c = binpow(a, d, p);
            if (c == 1) continue;
            int fl = 0;
            for (int q = 0; q < k && !fl; q++) {
                fl |= c == p - 1;
                c = __uint128_t(c) * c % p;
            }
            if (!fl) return 0;
        }
        return 1;
    }
};