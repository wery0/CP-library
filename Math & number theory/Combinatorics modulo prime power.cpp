template<typename T>
class combinatorics_mod_prime_power {
    static_assert(is_integral_v<T> && is_unsigned_v<T> || is_same_v<T, __uint128_t>);
    T p, a, P = 1;
    T carmichael, block_prod;
    vector<T> prod;

    // fast_mod<T> fmp, fmP;
    T mulmod(T x, T y) {
        return x * y % P;
        // return fmP.rem(x * y);
    }
    T binpow(T x, T k) {T o = 1; for (; k; x = mulmod(x, x), k >>= 1) if (k & 1) o = mulmod(o, x); return o;}
    T inv(T x) {return binpow(x, carmichael - 1);}

public:
    combinatorics_mod_prime_power() = default;
    //<O(p^a), O(p^a)>
    combinatorics_mod_prime_power(T p, T a): p(p), a(a) {
        static_assert(is_integral_v<T> && is_unsigned_v<T>);
        assert(p > 1);
        for (T i = 0; i < a; ++i, P *= p) assert(numeric_limits<T>::max() / p > P && "p^a should fit into T!");
        assert(numeric_limits<T>::max() / P > P * 2 && "2p^{2a} should fit into T!");
        // fmp = fast_mod<T>(p), fmP = fast_mod<T>(P);
        carmichael = (P - P / p) / (p != 2 || a < 3 ? 1 : 2);
        block_prod = p != 2 || a < 3 ? P - 1 : 1;
        prod.resize(P + 1, 1);
        for (T i = 1; i < P; ++i) {
            for (T j = 0; j + 1 < p; ++i, ++j) prod[i] = mulmod(prod[i - 1], i);
            prod[i] = prod[i - 1];
        }
    }

    //For a prime power p^a finds max t s. t. n! is divisiple by p^t and returns {(n! / p^t) mod p^a, t}
    //Complexity: O(log_p(n))
    pair<T, T> factorial(T n) {
        T res = a == 0 ? 0 : 1, t = 0;
        for (T x = n; x; t += x) {
            T q = x / P, r = x % P;
            // auto [q, r] = fmP.honest_quot_rem(x);
            res = mulmod(res, x < P ? prod[x] : q % 2 && block_prod == P - 1 ? P - prod[r] : prod[r]);
            x /= p;
            // x = fmp.honest_quot(x);
        }
        return {res < P ? res : res - P, t};
    }

    //For a prime power p^a finds max t s. t. C(n, k) is divisiple by p^t and returns {(C(n, k) / p^t) mod p^a, t}
    //Complexity: O(log_p(n))
    pair<T, T> C(T n, T k) {
        auto [r1, k1] = factorial(n);
        auto [r2, k2] = factorial(k);
        auto [r3, k3] = factorial(n - k);
        T res = mulmod(r1, mulmod(inv(r2), inv(r3)));
        return {res < P ? res : res - P, k1 - k2 - k3};
    }
};
