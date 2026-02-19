namespace MillerRabin {
    /*
    Complexity: O(ITER * log(n))
    Accuracy:
      - If n is prime, always determines that
      - If n is composite, determines that with >= 1 - 4^{-ITER} probability (on practice probability is much better)
    */
    template<typename T>
    bool is_prime(const T n, int ITER = 5) {
        assert(numeric_limits<T>::max() / n > n && "n^2 should fit into T!");
        if (n < 64) {return 2891462833508853932ll >> n & 1;}
        if (n % 2 == 0) return 0;
        T k = __builtin_ctzll(n - 1), d = (n - 1) >> k;
        static mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
        static uniform_int_distribution<T> gen(0, numeric_limits<T>::max());
        for (int i = 0; i < ITER; ++i) {
            T b = 2 + gen(rnd) % (n - 2), c = 1;
            for (T dd = d, pb = b; dd; dd >>= 1) {
                if (dd & 1) c = c * pb % n;
                pb = pb * pb % n;
            }
            if (c == 1) continue;
            int fl = 0;
            for (T q = 0; q < k && !fl; ++q) {
                fl |= c == n - 1;
                c = c * c % n;
            }
            if (!fl) return 0;
        }
        return 1;
    }
};
