namespace CRT {
    namespace pollardRho {
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

        uint64_t gcd(uint64_t x, uint64_t y) {
            while (x && y) {
                y %= x;
                swap(x, y);
            }
            return x + y;
        }

        uniform_int_distribution<uint64_t> gen(0, ULLONG_MAX);
        mt19937 rng(1);
        uint64_t pollard(const uint64_t n, uint64_t seed = gen(rng)) {
            if (n == 1) return 1;
            if (MillerRabin::is_prime(n)) return n;
            const uint64_t K = (gen(rng) << 1) | 1;
            auto func = [&](__uint128_t x) -> uint64_t {
                return (x * x + K) % n;
            };
            const uint64_t IT = 2e5, C = 64;
            uint64_t x = seed, y = seed, dvs = 1;
            for (int q = 0; dvs == 1 || dvs == n; ++q) {
                if (q == IT / C) return -1;
                uint64_t p = 1, sx = x, sy = y;
                for (int i = 0; i < C; ++i) {
                    x = func(x);
                    y = func(func(y));
                    uint64_t d = x > y ? x - y : y - x;
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

        void go(uint64_t x, vector<uint64_t>& res) {
            if (x == 1) return;
            uint64_t d = pollard(x);
            while (d == -1) d = pollard(x);
            if (d == x) res.push_back(x);
            else {
                go(d, res);
                go(x / d, res);
            }
        }

        vector<uint64_t> factorize(uint64_t x) {
            vector<uint64_t> ans;
            for (uint64_t i : {2, 3, 5, 7, 11, 13, 17, 19}) {
                while (x % i == 0) ans.push_back(i), x /= i;
            }
            go(x, ans);
            sort(ans.begin(), ans.end());
            return ans;
        }
    };

    int64_t solve_coprime(vector<uint64_t> mods, vector<uint64_t> remainders) {
        auto gcd = [&](auto&& gcd, int64_t a, int64_t b, int64_t& x, int64_t& y) {
            if (!a) {
                x = 0, y = 1;
                return b;
            }
            int64_t g = gcd(gcd, b % a, a, x, y);
            int64_t nx = y - x * (b / a), ny = x;
            x = nx, y = ny;
            return g;
        };
        auto extended_inv = [&](int64_t a, int64_t md) -> int64_t {
            int64_t x, y, g = gcd(gcd, a, md, x, y);
            if (g != 1) return -1;
            if (x < 0) x += (-x + md - 1) / md * md;
            x %= md;
            return x;
        };
        size_t n = mods.size();
        assert(remainders.size() == n);
        uint64_t P = 1, ans = 0;
        for (uint64_t p : mods) {
            assert(__gcd(P, p) == 1);
            P *= p;
        }
        for (size_t i = 0; i < n; ++i) {
            assert(mods[i] > 0);
            uint64_t u = P / mods[i];
            ans += remainders[i] * u % P * extended_inv(u, mods[i]);  //be careful with overflow, use binmul if need.
            ans %= P;
        }
        return ans;
    }

    int64_t solve_general(vector<uint64_t> mods, vector<uint64_t> remainders) {
        size_t n = mods.size(); assert(remainders.size() == n);
        for (size_t i = 0; i < n; ++i) {
            assert(mods[i] > 0);
            remainders[i] %= mods[i];
        }
        unordered_map<uint64_t, array<uint64_t, 2>> mp;
        for (size_t i = 0; i < n; ++i) {
            auto m = pollardRho::factorize(mods[i]);
            unordered_map<uint64_t, uint64_t> res;
            for (uint64_t p : m) {
                if (res.count(p)) res[p] *= p;
                else res[p] = p;
            }
            for (auto& [p, prod] : res) {
                if (!mp.count(p)) {
                    mp[p] = {prod, remainders[i] % prod};
                } else if (prod <= mp[p][0]) {
                    if (mp[p][1] % prod != remainders[i] % prod) return -1;
                } else {
                    if (remainders[i] % mp[p][0] != mp[p][1]) return -1;
                    mp[p] = {prod, remainders[i] % prod};
                }
            }
        }
        vector<uint64_t> nmod, nrem;
        for (auto& [x, p] : mp) {
            nmod.push_back(p[0]);
            nrem.push_back(p[1]);
        }
        return solve_coprime(nmod, nrem);
    }
};