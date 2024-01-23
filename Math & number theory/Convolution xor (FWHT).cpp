//Fast Walsh-Hadamard transform (FWHT)
//res[k] = sum({arr1[i] * arr2[j] | (i ^ j) == k})
//O(nlogn)
template<typename T>
vector<T> xor_convolution(vector<T> arr1, vector<T> arr2) {
    const size_t m = max(arr1.size(), arr2.size());
    const size_t n = m & (m - 1) ? 2 << __lg(m) : m;
    const T inv_n = (T)(1) / n;
    arr1.resize(n, (T)0);
    arr2.resize(n, (T)0);
    auto fwht = [&](vector<T>& a) {
        for (size_t d = 2; d <= n; d <<= 1) {
            const size_t md = d >> 1;
            for (size_t l = 0; l < n; l += md) {
                for (size_t i = 0; i < md; ++i, ++l) {
                    const T x = a[l + md];
                    a[l + md] = a[l] - x;
                    a[l] += x;
                }
            }
        }
    };
    fwht(arr1);
    fwht(arr2);
    for (size_t i = 0; i < n; ++i) arr1[i] *= arr2[i];
    fwht(arr1);
    for (T& val : arr1) val *= inv_n;
    return arr1;
}
