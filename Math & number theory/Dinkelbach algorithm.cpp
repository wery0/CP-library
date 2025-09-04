//Finds argmax(f(x) / g(x)) over x in X
/*
Requirements:
  f(x) is continuous and concave
  g(x) is continuous and convex, g(x) > 0 for all x in X
  f and g are continuous over X
  h(c) should find argmax(f(x) - c * g(x))
Complexity:
  O(log(nM)) iterations, where M = max(max(|f|, 1), max(|g|, 1))
Problem example:
  https://codeforces.com/blog/entry/146040
*/
template<typename T = long double>
T dinkelbach_algorithm(auto f, auto g, auto h, T eps = 1e-9) {
    static_assert(is_floating_point_v<T>);
    T x = 0, c = 0;
    for (size_t iter = 0; ;) {
        assert(++iter < 1000 && "Smth is probably not right, check your functions and eps.");
        x = h(c);
        if (abs(f(x) - c * g(x)) < eps) break;
        c = f(x) / g(x);
    }
    return x;
}
