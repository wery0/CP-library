//do_multiples == false: res[k] = sum({arr[i] | k % i == 0}), i. e. sum over divisors
//              == true: res[k] = sum({arr[i] | i % k == 0}), i. e. sum over multiples
//Assuming 1-indexation: res[5] = arr[0] + arr[1] + arr[2] + arr[5] when do_multiples == false
//O(nlog(n))
template<bool do_multiples, bool do_inverse, typename T>
vector<T> sum_over_divisors(const vector<T>& arr) {
    const size_t n = arr.size();
    vector<T> res = arr;
    if (do_inverse) {
        const size_t st = do_multiples ? n : 1;
        const size_t fn = do_multiples ? 0 : n + 1;
        const size_t df = do_multiples ? -1 : 1;
        for (size_t i = st; i != fn; i += df) {
            for (size_t j = i * 2; j <= n; j += i) {
                if constexpr(do_multiples) res[i - 1] -= res[j - 1];
                else res[j - 1] -= res[i - 1];
            }
        }
    } else {
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = i * 2; j <= n; j += i) {
                if constexpr(do_multiples) res[i - 1] += arr[j - 1];
                else res[j - 1] += arr[i - 1];
            }
        }
    }
    return res;
}
