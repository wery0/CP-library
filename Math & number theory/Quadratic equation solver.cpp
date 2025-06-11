//Finds all real solutions of equation ax^2 + bx + c = 0
template<typename T>
vector<T> solve_quadratic_equation(T a, T b, T c, T eps = 1e-12) {
    static_assert(is_floating_point_v<T>);
    T D = b * b - 4 * a * c;
    if (D < -eps) return {};
    if (D < eps) return {-b / (a * 2)};
    D = sqrtl(D);
    T x1 = (-b - D) / (a * 2);
    T x2 = (-b + D) / (a * 2);
    if (x1 > x2) swap(x1, x2);
    return {x1, x2};
}
