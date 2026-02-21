/*
Powerful number sieve, calculates sum_f(n)
Idea:
- Let f(n) be multiplicative, f = g * h (Dirichlet convolution) i. e. f(n) = sum(g(d)h(n/d) | d|n)
- g(n) is arbitrary function s. t. we can quickly calculate sum_g(n)
- h(n) is multiplicative function s. t. h(p) = 0
- sum_f(n) = sum(h(k) * sum_g(n / k) | k <= n)
- h(p) = 0 => let's bruteforce only k where h(k) != 0. There are O(sqrt(n)) such k.
Complexity: <O(sqrt(n) * O(sum_g)), O(1)>
Example:
  - f(n) = rad(n)
  - g(n) = n
  - h(p ^ k) = p - p ^ 2 for k >= 2
*/
template<typename T, typename N>
T powerful_number_sieve(const vector<int>& primes, auto h, auto sum_g, N n) {
    T res = 0;
    function<void(N, T, int)> go = [&](N k, T h_k, int i) {
        res += h_k * sum_g(T(n / k));
        for (int j = i; j < primes.size(); ++j) {
            N p = primes[j], pp = p * p, pw = 2;
            if (pp > n / k) break;
            for (N nk = k * pp; nk <= n; ++pw, nk *= p) {
                go(nk, h_k * h(T(p), T(pw)), j + 1);
                if (nk > n / p) break;
            }
        }
    };
    go(1, 1, 0);
    return res;
}
