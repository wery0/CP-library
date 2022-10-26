//num       = 20
//divisors  = [1,  2,  3, 4, 5, 6, 7, 11, 21]
//quotients = [20, 10, 6, 5, 4, 3, 2,  1, 0]
template<typename T>
vec<T> smallest_divisors_producing_all_possible_quotietns(T num) {
    vec<T> divisors;
    T divisor = 1;
    while (divisor <= num) {
        divisors.pb(divisor);
        T quotient = num / divisor;
        divisor = num / quotient + 1;
    }
    divisors.pb(num + 1);
    return divisors;
}