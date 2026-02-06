/*
For a prime power p^a finds max t s. t. n! is divisiple by p^t and returns {(n! / p^t) mod p^a, t}
Ideas:
  - p is prime <=> (p - 1)! = -1 (mod p) (Wilson's theorem)
  - (p ^ a - 1)! = 1 (mod p^a) for p = 2, a >= 3 and -1 otherwise (powers of p are excluded from product)
  - (p - 1)! = -1 (mod p) => n!(p - 1 - n)! = (-1)^n (mod p) so let's calculate min(n, p - 1 - n)! (2x speed up for a = 1)
  - We can iterate only over odd numbers since 2 * 4 * ... * 2n = 2^n * n! (2x speed up)
  - We can use instruction-level parallelism (ILP) (~x3.5 speed up, depends on hardware)
  - We can use faster methods for mulmod() (x4+ speed up, depends on the method)
Complexity: <O(min(n, p^a * log_p(n))), O(1)>
Also:
  - For a = 1 can be done in O(sqrt(p)log(p)) per query, see https://codeforces.com/blog/entry/63491
  - For a > 1 can be done in O(log_p(n) ^ 2 + log_p(n)klog(k)) per query with <O(pklog^2(k)), O(pklog(k))> precomputation, see https://codeforces.com/blog/entry/116681
*/
template<typename T>
pair<T, T> calc_factorial_mod_prime_power(const T N, const T p, const T a, T P = 1) {
    static_assert(is_integral_v<T> && is_unsigned_v<T>);
    assert(p > 1);
    if (P == 1) for (T i = 0; i < a; ++i, P *= p) assert(numeric_limits<T>::max() / p > P && "p^a should fit into T!");
    assert(numeric_limits<T>::max() / P > P * 2 && "2p^{2a} should fit into T!");
    // fast_mod<T> fmp(p), fmP(P);
    auto mulmod = [&](T x, T y) -> T {  //ensure that x is never divisible on p
        if (y % p == 0) return x;
        return x * y % P;
        // if (a != 1 && fmp.honest_rem(y) == 0) return x;
        // return fmP.rem(x * y);
    };
    auto calc = [&](T x, T k) {T o = 0; for (x /= k; x; x /= k) o += x; return o;};
    auto binpow = [&](T x, T k) -> T {T o = 1; for (; k; x = mulmod(x, x), k >>= 1) if (k & 1) o = mulmod(o, x); return o;};
    T carmichael = (P - P / p) / (p != 2 || a < 3 ? 1 : 2);
    auto inv = [&](T x) {return binpow(x, carmichael - 1);};
    T block_prod = p != 2 || a < 3 ? P - 1 : 1;
    function<T(T)> calc_prod = [&](T n) -> T {
        if (n < 2) return 1;
        if (n >= P) return mulmod(binpow(block_prod, n / P), calc_prod(n % P));
        if (n * 2 >= P && a == 1) return mulmod(((P - n) & 1 ? P - 1 : 1), inv(calc_prod(P - 1 - n)));
        const int B = 8;
        array<T, B> ilp; fill(ilp.begin(), ilp.end(), 1);
        T prod = 1, i = 1;
        for (int k = p == 2 ? 0 : __lg(n); k >= 0; --k) {
            const T R = ((n >> k) - 1) / 2;
            for (; i + B <= R; i += B) {
                for (int j = 0; j < B; ++j) {
                    ilp[j] = mulmod(ilp[j], (i + j) * 2 + 1);
                }
            }
            for (int j = 0; i <= R; ++i, ++j) ilp[j] = mulmod(ilp[j], i * 2 + 1);
            for (T r : ilp) prod = mulmod(prod, r);
        }
        if (p != 2) prod = mulmod(prod, binpow(2, calc(n, 2) - calc(n / p, 2)));
        return prod;
    };
    T res = a == 0 ? 0 : 1;
    for (T x = N; x; x /= p) res = mulmod(res, calc_prod(x));
    return {res, calc(N, p)};
}

//For a prime power p^a finds max t s. t. C(n, k) is divisiple by p^t and returns {(C(n, k) / p^t) mod p^a, t}
//Complexity: <O(min(n, p^a * log_p(n))), O(1)>
template<typename T>
pair<T, T> calc_choose_mod_prime_power(T n, T k, T p, T a, T P = 1) {
    static_assert(is_integral_v<T> && is_unsigned_v<T>);
    assert(p > 1 && a >= 0);
    if (k > n) return {0, 0};
    if (P == 1) for (T i = 0; i < a; ++i, P *= p) assert(numeric_limits<T>::max() / p > P && "p^a should fit into T!");
    assert(numeric_limits<T>::max() / P > P * 2 && "2p^{2a} should fit into T!");
    auto mulmod = [&](T x, T y) -> T {return x * y % P;};
    auto binpow = [&](T x, T k) -> T {T o = 1; for (; k; x = mulmod(x, x), k >>= 1) if (k & 1) o = mulmod(o, x); return o;};
    T carmichael = (P - P / p) / (p != 2 || a < 3 ? 1 : 2);
    auto inv = [&](T x) {return binpow(x, carmichael - 1);};
    auto [r1, k1] = calc_factorial_mod_prime_power<T>(n, p, a, P);
    auto [r2, k2] = calc_factorial_mod_prime_power<T>(k, p, a, P);
    auto [r3, k3] = calc_factorial_mod_prime_power<T>(n - k, p, a, P);
    return {mulmod(r1, mulmod(inv(r2), inv(r3))), k1 - k2 - k3};
}
