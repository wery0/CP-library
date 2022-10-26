vec<int> min_divisors(int n) {
    vec<int> mnd(n + 1);
    vec<int> primes;
    for (int q = 2; q <= n; ++q) {
        if (mnd[q] == 0) {
            primes.pb(q);
            mnd[q] = q;
        }
        for (int p : primes) {
            int c = p * q;
            if (p > mnd[q] || c >= n) break;
            mnd[c] = p;
        }
    }
    return mnd;
}