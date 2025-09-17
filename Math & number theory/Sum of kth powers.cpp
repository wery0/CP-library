//Calculates sum{i ^ k | 1 <= i <= n}
//O(k)
template<typename T>
T sum_kth_powers_matrix(T n, size_t k) {
    static constexpr size_t MX = 10;
    assert(n > 0);
    assert(k <= MX);
    static constexpr array<array<T, MX + 1>, MX + 1> C = {array<T, MX + 1>
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0},
        {-1, 0, 10, 15, 6, 0, 0, 0, 0, 0, 0},
        {0, -1, 0, 5, 6, 2, 0, 0, 0, 0, 0},
        {1, 0, -7, 0, 21, 21, 6, 0, 0, 0, 0},
        {0, 2, 0, -7, 0, 14, 12, 3, 0, 0, 0},
        {-3, 0, 20, 0, -42, 0, 60, 45, 10, 0, 0},
        {0, -3, 0, 10, 0, -14, 0, 15, 10, 2, 0},
        {5, 0, -33, 0, 66, 0, -66, 0, 55, 33, 6},
    };
    static constexpr array<T, MX + 1> D = {1, 2, 6, 4, 30, 12, 42, 24, 90, 20, 66};
    T s = 0, t = n;
    for (size_t i = 0; i <= k; ++i, t *= n) s += t * C[k][i];
    return s / D[k];
}

//Counts sum{i ^ k | 1 <= i <= n}
template<typename T>
T sum_kth_powers_explicit(T n, int k) {
    assert(n > 0 && k <= 10);
    if (k == 0) return n;
    if (k == 1) return n * (n + 1) / 2;
    if (k == 2) return n * (n + 1) * (2 * n + 1) / 6;
    if (k == 3) return n * n * (n + 1) * (n + 1) / 4;
    if (k == 4) return n * (n + 1) * (2 * n + 1) * (3 * n * n + 3 * n - 1) / 30;
    if (k == 5) return n * n * (n + 1) * (n + 1) * (2 * n * n + 2 * n - 1) / 12;
    if (k == 6) return n * (n + 1) * (2 * n + 1) * (3 * n * n * n * n + 6 * n * n * n - 3 * n + 1) / 42;
    if (k == 7) return n * n * (n + 1) * (n + 1) * (3 * n * n * n * n + 6 * n * n * n - n * n - 4 * n + 2) / 24;
    if (k == 8) return n * (n + 1) * (2 * n + 1) * (5 * n * n * n * n * n * n + 15 * n * n * n * n * n + 5 * n * n * n * n - 15 * n * n * n - n * n + 9 * n - 3) / 90;
    if (k == 9) return n * n * (n + 1) * (n + 1) * (n * n + n - 1) * (2 * n * n * n * n + 4 * n * n * n - n * n - 3 * n + 3) / 20;
    if (k == 10) return n * (n + 1) * (2 * n + 1) * (n * n + n - 1) * (3 * n * n * n * n * n * n + 9 * n * n * n * n * n + 2 * n * n * n * n - 11 * n * n * n + 3 * n * n + 10 * n - 5) / 66;
    return -1;
}
