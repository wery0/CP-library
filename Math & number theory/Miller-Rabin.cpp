namespace MillerRabin {
    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    uint64_t big_prod_mod(const uint64_t x, const uint64_t y, const uint64_t m) {
        uint64_t c = (long double)x * y / m;
        int64_t ans = int64_t(x * y - c * m) % int64_t(m);
        return ans < 0 ? ans + m : ans;
    }

    uint64_t binpow(uint64_t x, uint64_t k, const uint64_t mod) {
        uint64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = big_prod_mod(o, x, mod);
            x = big_prod_mod(x, x, mod);
        }
        return o;
    }

    bool is_prime(const uint64_t p) {
        if (p < 64) {return 2891462833508853932ull >> p & 1;}
        static const vector<uint64_t> A32 = {2, 7, 61};  //enough for all p < 4759123141
        static const vector<uint64_t> A64 = {2, 325, 9375, 28178, 450775, 9780504};
        uint64_t k = __builtin_ctzll(p - 1), d = (p - 1) >> k;
        for (uint64_t a : (p < 4759123141ull ? A32 : A64)) {
            if (a >= p) break;
            uint64_t c = binpow(a, d, p);
            if (c == 1) continue;
            int fl = 0;
            for (uint64_t q = 0; q < k && !fl; ++q) {
                fl |= c == p - 1;
                c = big_prod_mod(c, c, p);
            }
            if (!fl) return 0;
        }
        return 1;
    }
};
