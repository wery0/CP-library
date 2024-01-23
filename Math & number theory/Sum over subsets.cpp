//do_superset == false: res[k] = sum({arr[i] | (k & i) == i}), i. e. sum of subsets
//             == true: res[k] = sum({arr[i] | (k & i) == k}), i. e. sum of supersets
//Size of array could be arbitrary, not necessary power of 2
//O(nlog(n))
template<bool do_superset, bool do_inverse, typename T>
vector<T> sum_over_subsets(vector<T> arr) {
    const size_t n = arr.size();
    for (size_t b = 1; b < n; b <<= 1) {
        if constexpr(do_inverse) {
            if constexpr(do_superset) {
                for (size_t i = n - b - 1; i; --i) {
                    if (~i & b) arr[i] -= arr[i ^ b];
                }
                arr[0] -= arr[b];
            } else {
                for (size_t i = n - 1; i; --i) {
                    if (i & b) arr[i] -= arr[i ^ b];
                }
            }
        } else {
            for (size_t st = do_superset ? 0 : b; st < n; st += b << 1) {
                const size_t thr = do_superset ? min(st + b, n - b) : st + b;
                for (size_t i = st; i < thr; ++i) {
                    arr[i] += arr[i ^ b];
                }
            }
        }
    }
    return arr;
}
