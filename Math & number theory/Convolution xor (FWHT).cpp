//Fast Walsh-Hadamard transform
//Mod must be prime
//O(nlogn)
template<typename T>
vector<T> xor_convolution(vector<T> arr1, vector<T> arr2, const T mod) {
    if (arr1.empty() || arr2.empty()) return {};
    auto fwht = [&](size_t n, vector<T>& m) {
        for (size_t d = 2; d <= n; d <<= 1) {
            const size_t md = d >> 1;
            for (size_t l = 0; l < n; l += md) {
                for (size_t i = 0; i < md; ++i, ++l) {
                    const T x = m[l + md];
                    m[l + md] = m[l] + mod - x;
                    m[l] += x;
                }
            }
            for (size_t i = 0; i < n; ++i) m[i] -= m[i] < mod ? 0 : mod;
        }
    };
    auto binpow = [&](int64_t x, int64_t k) {
        int64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    size_t n = max(arr1.size(), arr2.size());
    n = n & (n - 1) ? 2 << __lg(n) : n;
    arr1.resize(n);
    arr2.resize(n);
    fwht(n, arr1);
    fwht(n, arr2);
    for (size_t i = 0; i < n; ++i) arr1[i] = 1ll * arr1[i] * arr2[i] % mod;
    fwht(n, arr1);
    const int64_t kek = binpow(n, mod - 2);
    for (auto& val : arr1) val = val * kek % mod;
    return arr1;
}
