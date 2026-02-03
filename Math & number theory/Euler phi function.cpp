template<typename T>
T euler_phi_function(T n) {
    T o = 1, p = 1, pr = 4;
    for (T x : factorize(n)) {
        if (x == pr) p *= x;
        else {
            o *= p - p / pr;
            p = pr = x;
        }
    }
    o *= p - p / pr;
    return o;
}
