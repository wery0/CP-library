//Solves diophantine equation Ax + By = C
/*
If solution exists returns {x, y, dx, dy} where:
  - x is minimal positive
  - dx > 0 or dx == 0 and dy > 0
  - (x + k * dx, y + k * dy) are all solutions
If solution doesn't exist, returns {0, 0, 0, 0}
*/
template<typename T>
array<T, 4> diophantine_solver(T A, T B, T C) {
    auto gcd = [](auto&& gcd, T a, T b, T& x, T& y) {
        if (!a) {
            x = 0, y = 1;
            return b;
        }
        T g = gcd(gcd, b % a, a, x, y);
        T nx = y - x * (b / a), ny = x;
        x = nx, y = ny;
        return g;
    };
    T x, y, g = gcd(gcd, abs(A), abs(B), x, y);
    if (A < 0) x *= -1;
    if (B < 0) y *= -1;
    if (C % g) return {0, 0, 0, 0};
    x *= C / g, y *= C / g;
    T dx = B / g, dy = -A / g;
    if (dx < 0 || dx == 0 && dy < 0) dx *= -1, dy *= -1;
    auto move = [&](T steps) {x += steps * dx, y += steps * dy;};
    if (dx) {
        move(-x / dx);
        if (x <= 0) move(1);
    }
    return {x, y, dx, dy};
}

//Finds number of solutions of diophantine equation Ax + By = C s. t. lx <= x <= rx and ly <= y <= ry
template<typename T>
T number_of_diophantine_solutions_in_range(T A, T B, T C, T lx, T rx, T ly, T ry) {
    auto [x, y, dx, dy] = diophantine_solver(A, B, C);
    if (dx == 0 && dy == 0) return 0;
    auto when_inside = [](T a, T da, T l, T r) -> array<T, 2> {
        if (da == 0) return l <= a && a <= r ? array<T, 2>{numeric_limits<T>::min(), numeric_limits<T>::max()} : array<T, 2>{1, 0};
        T t = 0;
        auto move = [&](T steps) {t += steps; a += steps * da;};
        move((l - a) / da);
        if (a < l) move(da > 0 ? 1 : -1);
        if (a > r) return {1, 0};
        T cnt = (r - a) / abs(da);
        return da > 0 ? array<T, 2>{t, t + cnt} : array<T, 2>{t - cnt, t};
    };
    auto [ltx, rtx] = when_inside(x, dx, lx, rx);
    auto [lty, rty] = when_inside(y, dy, ly, ry);
    T L = max(ltx, lty);
    T R = min(rtx, rty);
    return max<T>(0, R - L + 1);
}
