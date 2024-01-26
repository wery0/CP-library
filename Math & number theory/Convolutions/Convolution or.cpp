//res[k] = sum({arr1[i] * arr2[j] | (i | j) == k})
//O(nlog(n))
template<typename T>
vector<T> or_convolution(vector<T> arr1, vector<T> arr2) {
    const size_t m = max(arr1.size(), arr2.size());
    const size_t n = m & (m - 1) ? 2 << __lg(m) : m;
    arr1.resize(n, (T)0);
    arr2.resize(n, (T)0);
    arr1 = sum_over_subsets<0, 0>(arr1);
    arr2 = sum_over_subsets<0, 0>(arr2);
    for (size_t i = 0; i < n; ++i) arr1[i] *= arr2[i];
    arr1 = sum_over_subsets<0, 1>(arr1);
    return arr1;
}
