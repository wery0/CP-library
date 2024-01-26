//Assuming 1-indexation
//res[k] = sum({arr1[i] * arr2[j] | lcm(i, j) == k})
//O(nlog(n))
template<typename T>
vector<T> lcm_convolution(vector<T> arr1, vector<T> arr2) {
    const size_t n = max(arr1.size(), arr2.size());
    arr1.resize(n, (T)0);
    arr2.resize(n, (T)0);
    arr1 = sum_over_divisors<0, 0>(arr1);
    arr2 = sum_over_divisors<0, 0>(arr2);
    for (size_t i = 0; i < n; ++i) arr1[i] *= arr2[i];
    arr1 = sum_over_divisors<0, 1>(arr1);
    return arr1;
}
