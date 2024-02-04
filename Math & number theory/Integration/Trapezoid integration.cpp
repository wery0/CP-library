template<typename T = long double>
T trapezoid_integration(const T l, const T r, const size_t ITER, auto f) {
    const T STEP = (r - l) / ITER;
    T ans = 0;
    for (size_t i = 0; i < ITER; ++i) {
        ans += f(l + STEP * (i * 2 + 1) / 2);
    }
    return ans * STEP;
}
