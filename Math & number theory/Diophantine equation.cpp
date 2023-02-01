//Solves diophantine equation Ax + By = C.
//Returns solution with minimal positive x.
template<typename T>
pair<T, T> diophantine_solver(T A, T B, T C) {
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
    assert(C % g == 0);               //No solution exists in this case. Handle it.
    //if (C % g) return {-1, -1};     //Example of handling this case.
    x *= C / g, y *= C / g;
    T dx = B / g, dy = -A / g;
    T c = x / dx;
    x -= c * dx;
    y -= c * dy;
    if (x <= 0) {
        if (dx > 0) x += dx, y += dy;
        else x -= dx, y -= dy;
    }
    return {x, y};
}
