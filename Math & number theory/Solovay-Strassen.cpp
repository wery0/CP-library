namespace SolovayStrassen {
    template<typename T>
    T binpow(T x, T k, const T mod) {
        T o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    }

    template<typename T>
    bool check(T n, T b) {
        T bp = binpow(b, (n - 1) / 2, n);
        if (bp != n - 1 && bp != 0 && bp != 1) return 0;
        T js = jacobi_symbol(b, n);
        if (js == -1) js = n - 1;
        return bp == js;
    }

    /*
    Complexity: O(ITER * log(n))
    Accuracy:
      - If n is prime, always determines that
      - If n is composite, determines that with >= 1 - 2^{-ITER} probability
    */
    template<typename T>
    bool is_prime(T n, int ITER = 10) {
        assert(numeric_limits<T>::max() / n > n && "n^2 should fit into T!");
        if (n < 3) return n == 2;
        if (n % 2 == 0) return false;
        static mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
        static uniform_int_distribution<T> gen(0, numeric_limits<T>::max());
        for (int i = 0; i < ITER; ++i) {
            T b = 2 + gen(rnd) % (n - 2);
            if (gcd(b, n) > 1) return false;
            if (!check(n, b)) return false;
        }
        return true;
    }
}
