//Returns vector of all primes in range [1, n].
template<typename T>
vector<T> segmented_sieve_of_eratosphenes(T n) {
    if (n <= 62) {
        uint64_t mask = 2891462833508853932ull & ((1ull << (n + 1)) - 1);
        vector<T> ans; ans.reserve(__builtin_popcountll(mask));
        while (mask) {
            int x = __builtin_ctzll(mask);
            ans.push_back(x);
            mask ^= 1ull << x;
        }
        return ans;
    }
    T sqrtn = sqrtl(n);
    const T B = sqrtn + (sqrtn & 1);
    vector<T> primes = segmented_sieve_of_eratosphenes(B);
    vector<T> ans = primes;
    ans.reserve(n / log(n));
    vector<char> block(B);
    for (T l = 1 + B; l <= n; l += B) {
        fill(block.begin(), block.end(), 1);
        for (T p : primes) {
            T j = (l + p - 1) / p * p - l;
            for (j += j & 1 ? p : 0; j < B; j += p * 2) block[j] = 0;
        }
        const T gr = min(B, n - l + 1);
        for (T i = 0; i < gr; i += 2) {
            if (block[i]) ans.push_back(l + i);
        }
    }
    return ans;
}