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

        inline ull func(__uint128_t x, ull mod) {
            return (x * x + 1) % mod;
        }

        inline ull gcd(ull x, ull y) {
            for (; x && y;) {
                y %= x;
                swap(x, y);
            }
            return x + y;
        }

        ull pollard(ull n, ull seed = rnd()) {
            if (n == 1 || MillerRabin::is_prime(n)) return n;
            if (n % 2 == 0) return 2;
            if (n % 5 == 0) return 5;
            int IT = 200000;
            ull x = seed, y = seed, divisor = 1;
            for (int q = 0; divisor == 1 || divisor == n; q++) {
                if (q == IT) return -1;
                x = func(x, n);
                y = func(func(y, n), n);
                divisor = gcd(n, x < y ? y - x : x - y);
            }
            return divisor;
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
            go(x, res);
            sort(all(res));
            return res;
        }
    };

    ll solve_coprime(vec<ll> mod, vec<ll> rem) {
        auto gcd = [&](auto && gcd, ll a, ll b, ll & x, ll & y) {
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
            if (g != 1) return (ll)-1;
            if (x < 0) x += (-x + md - 1) / md * md;
            x %= md;
            return x;
        };
        ll a = mod.size();
        assert(rem.size() == a);
        ull P = 1, ans = 0;
        for (ull i : mod) {
            assert(__gcd(P, i) == 1);
            P *= i;
        }
        for (int q = 0; q < a; q++) {
            ll u = P / mod[q];
            ans += rem[q] * u % P * extended_inv(u, mod[q]);  //be careful with overflow, use binmul if need.
            ans %= P;
        }
        return ans;
    }

    ll solve_general(vec<ll> mod, vec<ll> rem) {
        ll a = mod.size(); assert(rem.size() == a);
        for (int q = 0; q < a; ++q) {
            if (rem[q] < 0) rem[q] += (-rem[q] + mod[q] - 1) / mod[q] * mod[q];
            assert(rem[q] >= 0);
            if (rem[q] >= mod[q]) rem[q] %= mod[q];
            assert(rem[q] < mod[q]);
        }
        umap<ll, pll> mp;
        for (int q = 0; q < a; ++q) {
            auto m = pollardRho::factorize(mod[q]);
            umap<ll, ll> res;
            for (ll i : m) {
                if (res.count(i)) res[i] *= i;
                else res[i] = i;
            }
            for (auto &[p, prod] : res) {
                if (!mp.count(p)) {
                    mp[p] = {prod, rem[q] % prod};
                } else if (prod <= mp[p].F) {
                    if (mp[p].S % prod != rem[q] % prod) return -1;
                } else {
                    if (rem[q] % mp[p].F != mp[p].S) return -1;
                    mp[p] = {prod, rem[q] % prod};
                }
            }
        }
        vec<ll> nmod, nrem;
        for (auto &p : mp) {
            nmod.pb(p.S.F);
            nrem.pb(p.S.S);
        }
        return solve_coprime(nmod, nrem);
    }
};