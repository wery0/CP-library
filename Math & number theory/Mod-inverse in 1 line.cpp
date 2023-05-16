//To prevent overflow, x^2 and m^2 should fit in T.
//If inverse value exists (<=> gcd(x, m) == 1), this function will always return it.
//If doesn't - runtime error (division by zero)
template<typename T>
T inv(T x, T m) {
    return x == 1 ? x : (1 - inv(m % x, x) * m) / x + m;
}