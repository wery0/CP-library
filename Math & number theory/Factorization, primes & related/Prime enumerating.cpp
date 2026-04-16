//Sieve of Eratosthenes with optimizations.
//Returns vector of all primes in range [1, n].
//Complexity: <O(nlog(log(n))), O(n)>
template<typename T>
vector<T> sieve_of_eratosthenes(T n) {
    if (n <= 2) {if (n == 2) return {2}; return {};}
    vector<T> ans = {2, 3}; ans.reserve(n * 1.08 / log(n));
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = 0;
    for (int w = 4; w <= n; w += 2) is_prime[w] = 0;
    for (int w = 9; w <= n; w += 6) is_prime[w] = 0;
    const int gr = sqrtl(n) + 1;
    for (int w = 6; w <= gr; w += 6) {
        for (int df : {-1, 1}) {
            const int q = w + df;
            if (q > n) break;
            if (!is_prime[q]) continue;
            for (int w = q * q; w <= n; w += q * 2) {
                is_prime[w] = 0;
            }
        }
    }
    for (int w = 6; w - 1 <= n; w += 6) {
        for (int df : {-1, 1}) {
            const int q = w + df;
            if (q <= n && is_prime[q]) ans.push_back(q);
        }
    }
    return ans;
}

//Calls function f() for each prime in range [L, R]
//<O(sqrt(R)loglog(sqrt(R)) + (R - L)loglog(R)), O(sqrt(R))>
template<typename T>
void enumerate_primes(T L, T R, auto f) {
    L = max(L, (T)2);
    if (L > R) return;
    T sqrtn = sqrtl(R);
    const T B = sqrtn + (sqrtn & 1);
    vector<T> primes = sieve_of_eratosthenes(B);
    for (T p : primes) {
        if (p >= L) f(p);
    }
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
            if (block[i]) f(l + i);
        }
    }
}
