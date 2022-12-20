//num       = 20
//divisors  = [1,  2,  3, 4, 5, 6, 7, 11, 21]
//quotients = [20, 10, 6, 5, 4, 3, 2,  1, 0]
template<typename T>
vector<T> smallest_divisors_producing_all_possible_quotietns(T num) {
    vector<T> divisors; divisors.reserve((int)(sqrt(num) * 2) + 1);
    T divisor = 1;
    while (divisor <= num) {
        divisors.push_back(divisor);
        T quotient = num / divisor;
        divisor = num / quotient + 1;
    }
    divisors.push_back(num + 1);
    return divisors;
}