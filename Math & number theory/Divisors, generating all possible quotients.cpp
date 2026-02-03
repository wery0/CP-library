/*
Example:
  n         = 20
  divisors  = [1,  2,  3, 4, 5, 6, 7, 11, 21]
  quotients = [20, 10, 6, 5, 4, 3, 2,  1, 0]
Complexity: O(|divisors|) ~ 2sqrt(n)
*/
template<typename T>
vector<T> smallest_divisors_producing_all_possible_quotietns(T n) {
    static_assert(is_integral_v<T>);
    assert(n > 0);
    vector<T> divisors; divisors.reserve(size_t(sqrtl(n) * 2) + 1);
    for (T divisor = 1; divisor <= n; ) {
        divisors.push_back(divisor);
        T quotient = n / divisor;
        divisor = n / quotient + 1;
    }
    divisors.push_back(n + 1);
    return divisors;
}
