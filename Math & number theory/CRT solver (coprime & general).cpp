namespace CRT {
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
            const ull K = (uniform_int_distribution<ull>(0, ULLONG_MAX)(rndll) << 1) | 1;
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

        void go(ull x, vector<ull>& res) {
            if (x == 1) return;
            ull d = pollard(x);
            for (; d == -1;) d = pollard(x);
            if (d == x) res.push_back(x);
            else {
                go(d, res);
                go(x / d, res);
            }
        }

        vector<ull> factorize(ull x) {
            vector<ull> res;
            for (ull i : {2, 3, 5, 7, 11, 13, 17, 19}) {
                while (x % i == 0) res.push_back(i), x /= i;
            }
            go(x, res);
            sort(all(res));
            return res;
        }
    };

    ll solve_coprime(vector<ll> mods, vector<ll> remainders) {
        auto gcd = [&](auto&& gcd, ll a, ll b, ll& x, ll& y) {
            if (!a) {
                x = 0, y = 1;
                return b;
            }
            ll g = gcd(gcd, b % a, a, x, y);
            ll nx = y - x * (b / a), ny = x;
            x = nx, y = ny;
            return g;
        };
        auto extended_inv = [&](ll a, ll md) {
            ll x, y, g = gcd(gcd, a, md, x, y);
            if (g != 1) return (ll) - 1;
            if (x < 0) x += (-x + md - 1) / md * md;
            x %= md;
            return x;
        };
        ll n = mods.size();
        assert(remainders.size() == n);
        ull P = 1, ans = 0;
        for (ull i : mods) {
            assert(__gcd(P, i) == 1);
            P *= i;
        }
        for (int q = 0; q < n; q++) {
            assert(mods[q] > 0);
            ll u = P / mods[q];
            ans += remainders[q] * u % P * extended_inv(u, mods[q]);  //be careful with overflow, use binmul if need.
            ans %= P;
        }
        return ans;
    }

    ll solve_general(vector<ll> mods, vector<ll> remainders) {
        ll n = mods.size(); assert(remainders.size() == n);
        for (int q = 0; q < n; ++q) {
            assert(mods[q] > 0);
            if (remainders[q] < 0) remainders[q] += (-remainders[q] + mods[q] - 1) / mods[q] * mods[q];
            assert(remainders[q] >= 0);
            if (remainders[q] >= mods[q]) remainders[q] %= mods[q];
            assert(remainders[q] < mods[q]);
        }
        umap<ll, pair<ll, ll>> mp;
        for (int q = 0; q < n; ++q) {
            auto m = pollardRho::factorize(mods[q]);
            umap<ll, ll> res;
            for (ll i : m) {
                if (res.count(i)) res[i] *= i;
                else res[i] = i;
            }
            for (auto& [p, prod] : res) {
                if (!mp.count(p)) {
                    mp[p] = {prod, remainders[q] % prod};
                } else if (prod <= mp[p].first) {
                    if (mp[p].second % prod != remainders[q] % prod) return -1;
                } else {
                    if (remainders[q] % mp[p].first != mp[p].second) return -1;
                    mp[p] = {prod, remainders[q] % prod};
                }
            }
        }
        vector<ll> nmod, nrem;
        for (auto& [x, p] : mp) {
            nmod.push_back(p.first);
            nrem.push_back(p.second);
        }
        return solve_coprime(nmod, nrem);
    }
};