//long double f(long double x) {return sin(x) * x;}
template<typename T = long double>
T trapezoid_integration(T l, T r, const int ITER) {
    const T STEP = (r - l) / ITER;
    T ans = 0;
    for (int i = 0; i < ITER; ++i) {
        ans += f(l + STEP * (i * 2 + 1) / 2);
    }
    return ans * STEP;
}