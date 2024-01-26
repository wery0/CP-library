//res[k] = sum({arr1[i] * arr2[j] | (i & j) == k})
//O(nlog(n))
template<typename T>
vector<T> and_convolution(vector<T> arr1, vector<T> arr2) {
    const size_t m = min(arr1.size(), arr2.size());
    const size_t n = max(arr1.size(), arr2.size());
    arr1.resize(n, (T)0);
    arr2.resize(n, (T)0);
    arr1 = sum_over_subsets<1, 0>(arr1);
    arr2 = sum_over_subsets<1, 0>(arr2);
    for (size_t i = 0; i < n; ++i) arr1[i] *= arr2[i];
    arr1 = sum_over_subsets<1, 1>(arr1);
    arr1.resize(m);
    return arr1;
}
