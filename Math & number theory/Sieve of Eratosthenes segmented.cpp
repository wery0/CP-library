//Returns vector of all primes in range [L, R].
//O(sqrt(R)loglog(sqrt(R)) + (R-L)loglog(R))
template<typename T>
vector<T> segmented_sieve_of_eratosthenes(T L, T R) {
    if (L > R) return {};
    if (R <= 62) {
        uint64_t mask = 2891462833508853932ull & (((1ull << (R - L + 1)) - 1)) << L;
        vector<T> ans; ans.reserve(__builtin_popcountll(mask));
        while (mask) {
            ans.push_back(__builtin_ctzll(mask));
            mask &= mask - 1;
        }
        return ans;
    }
    T sqrtn = sqrtl(R);
    const T B = sqrtn + (sqrtn & 1);
    vector<T> primes = segmented_sieve_of_eratosthenes<T>(1, B);
    vector<T> ans(lower_bound(primes.begin(), primes.end(), L), primes.end());
    ans.reserve((R - L) / log(R));
    vector<char> block(B);
    for (T l = 1 + max(B, (L - 1) / B * B); l <= R; l += B) {
        fill(block.begin(), block.end(), 1);
        for (T p : primes) {
            T j = (l + p - 1) / p * p - l;
            for (j += j & 1 ? p : 0; j < B; j += p * 2) block[j] = 0;
        }
        const T gr = min(B, R - l + 1);
        T i = l < L ? L - l : 0;
        for (i += i & 1; i < gr; i += 2) {
            if (block[i]) ans.push_back(l + i);
        }
    }
    return ans;
}