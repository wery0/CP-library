//long double f(long double x) {return sin(x) * x;}
//const long double EPS = 1e-9; 
template<typename T = long double>
T romberg(T l, T r) {
    T d = r - l;
    vector<T> t = {d * (f(l) + f(r)) / 2};
    for (int64_t k = 1, i = 1; ; k *= 2, d /= 2, ++i) {
        T last = t.back(), x = l + d / 2, cur = 0;
        for (int64_t j = 0; j < k; ++j) cur += f(x), x += d;
        cur = (t[0] + d * cur) / 2;
        T k1 = (T)4.0 / 3.0, k2 = (T)1.0 / 3.0;
        for (int64_t j = 0; j < i; ++j) {
            T temp = k1 * cur - k2 * t[j];
            t[j] = cur; cur = temp;
            k2 /= 4 * k1 - k2;
            k1 = k2 + 1;
        }
        t.push_back(cur);
        if (abs(last - cur) < EPS) break;
    }
    return t.back();
}
