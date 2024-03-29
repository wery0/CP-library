//Faster than usual ternary search in 2log_{1.5}(FI) ~ 2.37 times.
template<typename T = long double>
T ternary_search_with_golden_ratio_argmin(auto f, T l, T r) {
    static_assert(is_floating_point_v<T>);
    static constexpr T FI = 1.6180339887498948482045868343656381177203;    //(sqrt(5) + 1) / 2.0
    T p1 = l + (r - l) / (FI + 1), p2 = r - (r - l) / (FI + 1);
    T v1 = f(p1), v2 = f(p2);
    for (; r - l > 1e-9;) {    //Change epsilon, if need, or put fixed amount of iterations
        if (v1 < v2) {
            r = p2;
            p2 = p1, v2 = v1;
            p1 = l + (r - l) / (FI + 1), v1 = f(p1);
        } else {
            l = p1;
            p1 = p2, v1 = v2;
            p2 = r - (r - l) / (FI + 1), v2 = f(p2);
        }
    }
    return (l + r) / 2;
}
