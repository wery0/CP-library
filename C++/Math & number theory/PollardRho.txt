namespace pollardRho {
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

    inline ull gcd(ull x, ull y) {
        for (; x && y;) {
            y %= x;
            swap(x, y);
        }
        return x + y;
    }

    ull pollard(const ull n, ull seed = rnd()) {
        if (n == 1) return 1;
        if (MillerRabin::is_prime(n)) return n;
        const ull K = (uid<ull>(0, ULLONG_MAX)(rndll) << 1) | 1;
        auto func = [&](__uint128_t x) -> ull {
            return (x * x + K) % n;
        };
        const ull IT = 2e5, C = 64;
        ull x = seed, y = seed, dvs = 1;
        for (int q = 0; dvs == 1 || dvs == n; ++q) {
            if (q == IT / C) return -1;
            ull p = 1, sx = x, sy = y;
            for (int i = 0; i < C; ++i) {
                x = func(x);
                y = func(func(y));
                ull d = x > y ? x - y : y - x;
                d %= n;
                if (d) p = (__uint128_t)p * d % n;
            }
            if (p == 0) {
                for (x = sx, y = sy; dvs == 1 || dvs == n;) {
                    x = func(x);
                    y = func(func(y));
                    dvs = gcd(n, x > y ? x - y : y - x);
                }
                return dvs;
            }
            dvs = gcd(n, p);
        }
        return dvs;
    }

    void go(ull x, vec<ull> &res) {
        if (x == 1) return;
        ull d = pollard(x);
        for (; d == -1;) d = pollard(x);
        if (d == x) res.pb(x);
        else {
            go(d, res);
            go(x / d, res);
        }
    }

    vec<ull> factorize(ull x) {
        vec<ull> res;
        for (ull i : {2, 3, 5, 7, 11, 13, 17, 19}) {
            while (x % i == 0) res.pb(i), x /= i;
        }
        go(x, res);
        sort(all(res));
        return res;
    }
};