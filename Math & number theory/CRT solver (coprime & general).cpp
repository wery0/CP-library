namespace CRT {

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
            unordered_map<uint64_t, uint64_t> res;
            for (uint64_t p : factorize(mods[i])) {
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
