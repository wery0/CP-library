//Uses newtons method, works for any a >= 0, k >= 0. Be careful with a < 0.
template<typename T>
T kth_root(T a, T k, size_t iterations = 1000) {
    static_assert(is_floating_point_v<T>);
    assert(k >= 0);
    if (a == 0 || k == 0) return a == k;
    if (k < 1) return powl(a, 1.0 / k);
    T x = a < 1 ? 1 : a / k;  //better approximation -> faster convergence
    T px = x;
    for (size_t it = 0; it < iterations; ++it) {
        T pw = powl(x, k - 1);
        T nx = x + (a - x * pw) / (k * pw);
        int fl = nx == x || nx == px;
        px = x, x = nx;
        if (fl) break;
    }
    return (px + x) / 2;
}
