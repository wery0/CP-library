/*
    Tries to find x in range [L, R] s. t. f(x) is minimal
    Use when f() is almost ternary
    Let T = N ^ 2 * step * log_2((R - L + 1) / block_size) * block_size
    Then kT = O(T) calls of f() gonna happen.

    For any block_size and R - L < 10 ^ 40:
     ___________________________________________________________
    |              |              |              |              |
    |              |    N = 10    |    N = 50    |   N = 100    |
    |______________|______________|______________|______________|
    |              |              |              |              |
    |  step = 0.1  |   k < 4.35   |   k < 1.62   |   k < 1.30   |
    |______________|______________|______________|______________|
    |              |              |              |              |
    |  step = 0.5  |   k < 1.74   |   k < 1.12   |   k < 1.04   |
    |______________|______________|______________|______________|
    |              |              |              |              |
    |  step = 1    |   k < 1.41   |   k < 1.05   |   k < 1.00   |
    |______________|______________|______________|______________|


    Victims:
    https://codeforces.com/contest/2035/problem/E
    https://imcs.dvfu.ru/cats/static/problem_text-cpid-7515571.html?sid=56OyI1rk3u5bTN0ECZsfs7VX5Slnhj
*/
template <typename T>
T integral_keksearch_argmin(auto f, T L, T R, size_t N = 100, double step = 0.5, size_t block_size = 1, size_t block_eval_mode = 0) {
    static_assert(is_integral_v<T>);
    assert(L <= R);
    assert(N > 0 && step > 0 && block_size > 0);
    assert(block_eval_mode <= 1);
    using U = decltype(f(L));

    T best_arg = L;
    U best_val = f(L);
    auto upd = [&](T arg, U val) {if (val < best_val) best_val = val, best_arg = arg;};
    
    T expected_calls = N * N * step * logl(double(R - L + 1) / block_size) * block_size;
    if (expected_calls > R - L || R - L < block_size * 3) {
        while (++L <= R) upd(L, f(L));
        return best_arg;
    }

    while (++L % block_size) upd(L, f(L));
    while (R % block_size) upd(R, f(R)), --R;
    T bl = L / block_size, br = R / block_size - 1;
    upd(R, f(R));

    auto g = [&](T l) {
        l *= block_size;
        U val = block_eval_mode == 0 ? 0 : numeric_limits<U>::max();
        for (size_t i = 0; i < block_size; ++i, ++l) {
            U tyt = f(l);
            if (block_eval_mode == 0) val += tyt;
            else val = min(val, tyt);
            upd(l, tyt);
        }
        return val;
    };
    for (int iter = 0; iter <= N; ++iter) {
        double i = 2.01 + iter * step;
        T l = bl, r = br;
        while (r - l > i * 3) {
            T cut = (r - l) / i;
            T p1 = l + cut;
            T p2 = r - cut;
            assert(p1 < p2);
            (g(p1) < g(p2) ? r = p2 : l = p1);
        }
        while (l <= r) g(l++);
    }
    return best_arg;
};
