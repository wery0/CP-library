ld trapezoid_integration(ld l, ld r, const int ITER) {
    const ld STEP = (r - l) / ITER;
    ld o = 0;
    for (int i = 0; i < ITER; ++i) {
        o += f(l + STEP * (i * 2 + 1) / 2);
    }
    return o * STEP;
}