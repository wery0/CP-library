namespace PollardRho {
    uint64_t gcd(uint64_t x, uint64_t y) {
        while (x && y) {
            y %= x;
            swap(x, y);
        }
        return x + y;
    }

    uint64_t pollard(const uint64_t n) {
        static uniform_int_distribution<uint64_t> gen(0, ULLONG_MAX);
        static mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        if (n == 1) return 1;
        if (is_prime(n)) return n;
        const uint64_t K = (gen(rng) << 1) | 1;
        auto func = [&](__uint128_t x) -> uint64_t {
            return (x * x + K) % n;
        };
        const uint64_t IT = 2e5, C = 64;
        uint64_t x = gen(rng), y = x, dvs = 1;
        for (uint64_t iter = 0; dvs == 1 || dvs == n; ++iter) {
            if (iter == IT / C) return -1;
            uint64_t p = 1, sx = x, sy = y;
            for (uint64_t i = 0; i < C; ++i) {
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

    //n = p_1 ^ k_1 * p_2 ^ k_2 *... Returns vector of pairs {p_i, k_i}
    //O(n ^ {1 / 4})
    vector<array<uint64_t, 2>> factorize(uint64_t n) {
        vector<uint64_t> ans;
        for (uint64_t i : {2, 3, 5, 7, 11, 13, 17, 19}) {
            while (n % i == 0) ans.push_back(i), n /= i;
        }
        go(n, ans);
        sort(ans.begin(), ans.end());
        vector<array<uint64_t, 2>> res;
        uint64_t pr = 0, c = 0;
        for (auto p : ans) {
            if (pr == p) ++c;
            else {
                if (c) res.push_back({pr, c});
                c = 1;
            }
            pr = p;
        }
        if (c) res.push_back({pr, c});
        return res;
    }
};
//Usage: PollardRho::factorize(x)
