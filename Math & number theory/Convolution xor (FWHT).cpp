//Mod must be prime.
//O(nlogn)
template<typename T>
vector<T> xor_convolution(vector<T> arr1, vector<T> arr2, const ll mod) {
    if (arr1.empty() || arr2.empty()) return {};
    auto fwht = [&](int a, vector<T>& m) {
        for (int d = 2; d <= a; d <<= 1) {
            const int md = d >> 1;
            for (int l = 0; l < a; l += md) {
                for (int q = 0; q < md; ++q, ++l) {
                    const T x = m[l + md];
                    m[l + md] = m[l] + mod - x;
                    m[l] += x;
                }
            }
            for (int q = 0; q < a; ++q) m[q] -= m[q] < mod ? 0 : mod;
        }
    };
    auto binpow = [&](ll x, ll k) {
        ll o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    int n = max(arr1.size(), arr2.size());
    n = n & (n - 1) ? 2 << __lg(n) : n;
    arr1.resize(n);
    arr2.resize(n);
    fwht(n, arr1);
    fwht(n, arr2);
    for (int q = 0; q < n; ++q) arr1[q] = 1ll * arr1[q] * arr2[q] % mod;
    fwht(n, arr1);
    const ll in = binpow(n, mod - 2);
    for (int q = 0; q < n; q++) arr1[q] = arr1[q] * in % mod;
    return arr1;
}