namespace TrialDivision {
    //n = p_1 ^ k_1 * p_2 ^ k_2 *... Returns vector of pairs {p_i, k_i}
    //O(sqrt(n)) with good constant
    template<typename T>
    vector<array<T, 2>> factorize(T n) {
        assert(n > 0);
        vector<array<T, 2>> res;
        auto try_divide = [&](T d) {
            T cnt = 0;
            while (n % d == 0) n /= d, ++cnt;
            if (cnt) res.push_back({d, cnt});
        };
        try_divide(2);
        try_divide(3);
        const T thr = sqrtl(n) + 2;
        for (T i = 6; i < thr; i += 6) {
            try_divide(i - 1);
            try_divide(i + 1);
        }
        if (n > 1) res.push_back({n, 1});
        return res;
    }

    //O(sqrt(n)) with good constant
    template<typename T>
    bool is_prime(const T& n) {
        if (n < 4) return n == 2 || n == 3;
        if (n % 2 == 0 || n % 3 == 0) return false;
        const T thr = sqrtl(n) + 2;
        for (T i = 6; i < thr; i += 6) {
            if (n % (i - 1) == 0) return false;
            if (n % (i + 1) == 0) return false;
        }
        return true;
    }
};
