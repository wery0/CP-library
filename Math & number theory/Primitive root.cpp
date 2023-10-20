//Calculates minimal g, s. t. its powers modulo n run over all numbers coprime to n
//Returns n, if no answer exists
//It exist if and only if n in {1, 2, 4}, or n is p ^ k, or n is 2 * p ^ k, p > 2 is prime
//O(g * log(phi) * log(n)), g is small usually
template<typename T>
T primitive_root(T n, bool ensure_it_exists) {
    static_assert(is_integral_v<T>);
    if (ensure_it_exists) {
        if ((n & (n - 1)) == 0) {
            if (n == 0 || n > 4) return n;
            if (n == 1) return 0;
        } else {
            T x = n;
            if (x % 4 == 0) return n;
            if (x % 2 == 0) x /= 2;
            auto check = [&](T b) {assert(b > 1); T c = n; while (c % b == 0) c /= b; return c == 1;};
            for (int pw = __lg(x); pw > 1; --pw) {
                T base = powl(x, (long double)1.0 / pw);
                if (check(base)) {x = base; break;}
                if (check(base + 1)) {x = base + 1; break;}
            }
            if (!is_prime(x)) return n;
        }
    }
    auto binpow = [&](T x, T k) {  //Be careful with overflow
        T o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % n;
            x = x * x % n;
        }
        return o;
    };
    T phi = euler_phi_function(n);
    auto fc = factorize(phi);
    set<T> sfc(fc.begin(), fc.end());
    for (T g = 1; g < n; ++g) {
        bool fl = true;
        for (T p : sfc) {
            fl &= binpow(g, phi / p) != 1;
            if (!fl) break;
        }
        if (fl) return g;
    }
    return n;
}
