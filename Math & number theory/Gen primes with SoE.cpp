//Fast Sieve of Eratosthenes with const optimizations.
//Returns vector of all primes in range [1, n].
vector<int> gen_primes(int n) {
    if (n <= 2) {if (n == 2) return {2}; return {};}
    vector<int> o = {2, 3}; o.reserve(n / log(n));
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = 0;
    for (int w = 4; w <= n; w += 2) is_prime[w] = 0;
    for (int w = 3; w <= n; w += 6) is_prime[w] = 0;
    const int gr = sqrtl(n) + 1;
    for (int w = 6; w <= gr; w += 6) {
        for (int df : { -1, 1}) {
            const int q = w + df;
            if (q > n) break;
            if (!is_prime[q]) continue;
            for (int w = q * q; w <= n; w += q * 2) {
                is_prime[w] = 0;
            }
        }
    }
    for (int w = 6; w - 1 <= n; w += 6) {
        for (int df : { -1, 1}) {
            const int q = w + df;
            if (q <= n && is_prime[q]) o.push_back(q);
        }
    }
    return o;
}