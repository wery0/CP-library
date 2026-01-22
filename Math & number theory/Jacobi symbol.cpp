//O(log(min(a, b)))
template<typename T>
int jacobi_symbol(T a, T b) {
    assert(b > 1 && b % 2 == 1);
    if (gcd(a, b) != 1) return 0;
    int r = 1;
    if (a < 0) {
        a = -a;
        if (b % 4 == 3) r *= -1;
    }
    while (a > 0) {
        int t = __builtin_ctzll(a);
        a >>= t;
        if (t & 1) {
            T rem = b % 8;
            if (rem == 3 || rem == 5) r *= -1;
        }
        if (a % 4 == 3 && b % 4 == 3) r *= -1;
        T tmp = a;
        a = b % a;
        b = tmp;
    }
    return r;
}
