//Sieve of Sundaram with optimizations
//Idea: Odd composite integer x can be written as 2t + 1 = (2i + 1)(2j + 1) => t = i + j + 2ij => bruteforce (i, j) and mark these numbers as composite.
//Returns vector of all primes in range [1, n]
//Complexity: <O(nlog(log(n))), O(n)>
template<typename T>
vector<T> sieve_of_sundaram(T n) {
    if (n < 2) return {}; n = (n - 1) / 2;
    vector<bool> is_prime(n + 1, 1);
    for (T i = 1; i * i * 2 <= n; ++i) {
        if (!is_prime[i]) continue;
        for (T j = i * 2 + i * i * 2; j <= n; j += i * 2 + 1) {
            is_prime[j] = 0;
        }
    }
    vector<T> ans = {2}; ans.reserve(n * 2.1 / log(n));
    for (T i = 1; i <= n; ++i) {
        if (is_prime[i]) ans.push_back(i * 2 + 1);
    }
    return ans;
}
