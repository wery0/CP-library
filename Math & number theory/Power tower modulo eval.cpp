namespace PowerTowerEval {
    template<typename T>
    T crt_solve_coprime(vector<T> mods, vector<T> remainders) {
        using TS = make_signed_t<T>;
        using TU = make_unsigned_t<T>;
        auto gcd = [&](auto&& gcd, TS a, TS b, TS& x, TS& y) {
            if (!a) {
                x = 0, y = 1;
                return b;
            }
            TS g = gcd(gcd, b % a, a, x, y);
            TS nx = y - x * (b / a), ny = x;
            x = nx, y = ny;
            return g;
        };
        auto extended_inv = [&](TS a, TS md) -> TS {
            TS x, y, g = gcd(gcd, a, md, x, y);
            if (g != 1) return -1;
            if (x < 0) x += (-x + md - 1) / md * md;
            x %= md;
            return x;
        };
        size_t n = mods.size();
        TU P = 1, ans = 0;
        for (TU p : mods) P *= p;
        for (size_t i = 0; i < n; ++i) {
            TU u = P / mods[i];
            ans += remainders[i] * u % P * extended_inv(u, mods[i]);
            ans %= P;
        }
        return ans;
    }

    template<typename T>
    vector<array<T, 2>> factorize(T n) {
        static unordered_map<T, vector<array<T, 2>>> fc_cache;
        assert(n > 0);
        auto& cached = fc_cache[n];
        if (cached.size()) return cached;
        vector<array<T, 2>> res;
        auto try_divide = [&](T d) {
            T cnt = 0;
            while (n % d == 0) n /= d, ++cnt;
            if (cnt) res.push_back({d, cnt});
        };
        try_divide(2);
        try_divide(3);
        const T thr = sqrtl(n) + 2;
        for (T i = 6; i < thr; i += 6) {
            try_divide(i - 1);
            try_divide(i + 1);
        }
        if (n > 1) res.push_back({n, 1});
        return cached = res;
    }

    template<typename T>
    T binpowmod(T x, T k, T mod) {
        T o = 1;
        while (k) {
            if (k & 1) o = o * x % mod;
            if (k >>= 1) x = x * x % mod;
        }
        return o;
    }

    template<typename T>
    T binpow(T x, T k) {
        T o = 1;
        while (k) {
            if (k & 1) o *= x;
            if (k >>= 1) x *= x;
        }
        return o;
    }

    template<typename T>
    T binpow_ub(T x, T k, T ub) {
        T o = 1;
        while (k) {
            if (o * x >= ub) return ub;
            if (k & 1) o *= x;
            if (k >>= 1) x *= x;
        }
        return o;
    }

    template<typename T>
    T euler_phi_function(T n) {
        static unordered_map<T, T> phi_cache;
        T& cached = phi_cache[n];
        if (cached) return cached;
        T res = 1;
        for (auto [d, c] : factorize<T>(n)) {
            T pw = binpow(d, c);
            res *= pw - pw / d;
        }
        return cached = res;
    }

    template<typename T>
    T eval_tower(const vector<T>& tower, size_t i, T ub) {
        if (i == tower.empty()) return min((T)1, ub);
        if (i + 1 == tower.size()) return min((T)1, tower[i]);
        T exponent = 1;
        const auto lst = tower.rbegin() + tower.size() - i;
        for (auto it = tower.rbegin(); it != lst && exponent < ub; ++it) {
            exponent = binpow_ub(*it, exponent, ub);
        }
        return min(exponent, ub);
    }

    template<typename T> T eval_tower_any_mod(const vector<T>&, size_t, T);

    template<typename T>
    T eval_tower_prime_power_mod(const vector<T>& tower, size_t i, T p, T pw) {
        T pp = binpow(p, pw), tb = tower[i];
        T g = gcd(p, tb), et = 0;
        T coprime_part = binpowmod(tb / g, eval_tower_any_mod(tower, i + 1, euler_phi_function(pp)), pp);
        if (g != 1) {
            assert(g == p);
            et = eval_tower(tower, i + 1, pw);
        }
        return et < pw ? binpow(p, et) * coprime_part % pp : 0;
    }

    template<typename T>
    T eval_tower_any_mod(const vector<T>& tower, size_t i, T mod) {
        if (i == tower.size()) return 1;
        T tb = tower[i];
        if (i + 1 == tower.size()) return tb % mod;
        if (i + 2 == tower.size()) return binpowmod(tb, tower.back(), mod);
        assert(tb > 1);
        vector<T> mods, rems;
        for (auto [p, c] : factorize<T>(mod)) {
            T pp = binpow(p, c);
            mods.push_back(pp);
            rems.push_back(eval_tower_prime_power_mod(tower, i, p, c));
        }
        return crt_solve_coprime(mods, rems);
    }

    //Calculates tower[0] ^ (tower[1] ^ (tower[2] ^ ...)) % mod
    //Algorithm is based on Euler's theorem: a ^ phi(n) == 1 (mod n) for coprime a and n
    //Complexity: ??? (Highly depends on factorization function. Can eval towers of 1e7 size by ~1e9 modulo in ~200 ms when using trial division factorization algorithm)
    //Constraint: tower[i] ^ 2 and mod ^ 2 should fit into T
    template<typename T>
    T power_tower_eval_modulo(vector<T> tower, T mod) {
        assert(mod > 0);
        if (mod == 1) return 0;
        for (size_t i = 0; i < tower.size(); ++i) {
            assert(tower[i] >= 0);
            if (tower[i] <= 1) {
                T val = 1;
                for (auto it = tower.rbegin(), lst = tower.rbegin() + tower.size() - i; it != lst; ++it) {
                    val = *it ? 1 : !val;
                }
                tower.erase(tower.begin() + i + 1, tower.end());
                tower[i] = val;
            }
        }
        if (tower.size() <= 1) return tower.empty() ? 1 : tower[0] % mod;
        if (tower.back() <= 1) {
            int fl = tower.back() == 0;
            tower.pop_back();
            if (fl) tower.pop_back();
        }
        return eval_tower_any_mod(tower, 0, mod);
    }
};
//Usage: PowerTowerEval::power_tower_eval_modulo(tower, mod)
