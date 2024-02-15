//Solves system of equations {x % mods[i] = remainders[i], 0 <= i < n}
//Contsraint: gcd(mods[i], mods[j]) == 1 for i != j
//Complexity: O(nlog(C))
template<typename T>
T crt_solve_coprime(vector<T> mods, vector<T> remainders) {
    using TS = make_signed_t<T>;
    using TU = make_unsigned_t<T>;
    auto gcd = [&](auto && gcd, TS a, TS b, TS & x, TS & y) {
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
    assert(remainders.size() == n);
    TU P = 1, ans = 0;
    for (TU p : mods) {
        assert(__gcd(P, p) == 1);
        P *= p;
    }
    for (size_t i = 0; i < n; ++i) {
        assert(mods[i] > 0);
        TU u = P / mods[i];
        ans += remainders[i] * u % P * extended_inv(u, mods[i]);  //be careful with overflow, use binmul if need.
        ans %= P;
    }
    return ans;
}

//Solves system of equations {x % mods[i] = remainders[i], 0 <= i < n}
//Complexity: O(nlog(C) + factorization of mods)
template<typename T>
T crt_solve_general(vector<T> mods, vector<T> remainders) {
    using TU = make_unsigned_t<T>;
    size_t n = mods.size(); assert(remainders.size() == n);
    for (size_t i = 0; i < n; ++i) {
        assert(mods[i] > 0);
        remainders[i] %= mods[i];
    }
    unordered_map<TU, array<TU, 2>> mp;
    for (size_t i = 0; i < n; ++i) {
        unordered_map<TU, TU> res;
        for (TU p : factorize(mods[i])) {
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
    vector<TU> nmod, nrem;
    for (auto& [x, p] : mp) {
        nmod.push_back(p[0]);
        nrem.push_back(p[1]);
    }
    return crt_solve_coprime(nmod, nrem);
}
