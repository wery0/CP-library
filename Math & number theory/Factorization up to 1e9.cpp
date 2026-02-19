__attribute__((noinline)) void factor_helper(vector<uint32_t>& v, uint32_t& x, uint32_t y) {
    do {
        v.push_back(y);
        x /= y;
    } while (x % y == 0);
}

constexpr int N = 1e9;
constexpr int maxp = sqrtl(N);
constexpr int P = 3401; // number of primes below maxp

constexpr auto primes = []() constexpr {
    array<int, P> res{};
    array<int, maxp> comp{};
    for (int idx = 0, p = 2; p < maxp; ++p) {
        if (!comp[p]) {
            res[idx++] = p;
            for (int j = p; j < maxp; j += p) {
                comp[j] = 1;
            }
        }
    }
    return res;
}();

template<int L = 0, int R = P, int M = (L + R) / 2>
constexpr void fact_p(vector<uint32_t>& v, uint32_t& x) {
    if constexpr (L + 1 < R) {
        fact_p<L, M>(v, x);
        if (L + 10 < R && x < primes[M] * primes[M]) return;
        fact_p<M, R>(v, x);
    } else if (x % primes[L] == 0) {
        factor_helper(v, x, primes[L]);
    }
}

/*
Compile-time optimized factorization of small numbers. Source: //https://codeforces.com/blog/entry/117622
Complexity: O(sqrt(x) / log(x)) with very good constant
*/
vector<uint32_t> factorize(uint32_t x) {
    vector<uint32_t> v;
    fact_p<0, P>(v, x);
    if (x > 1) v.push_back(x);
    return v;
}
