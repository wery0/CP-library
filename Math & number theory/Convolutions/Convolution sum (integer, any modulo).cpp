//Uses CRT theorem to calculate answer by any modulo
//O(|mods| * T(sum_convolution)) 
template<typename T, typename H = __uint128_t>
vector<T> sum_convolution_arbitrary_mod(vector<T>& arr1, vector<T>& arr2, const T mod) {
    assert(numeric_limits<T>::max() / mod >= mod && "mod * mod should fit into T!");
    auto powmod = [&](T x, T k, T mod) {T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x % mod; x = x * x % mod;} return o;};
    static const vector<T> mods = {167772161, 469762049, 754974721};  //product ~5.95e25. Coefficients of result should fit into this number.
    static const H P = accumulate(mods.begin(), mods.end(), (H)1, multiplies<H>());
    static const size_t sz = mods.size();
    vector<H> mnoj(sz);
    for (size_t i = 0; i < sz; ++i) {
        const T m = mods[i];
        mnoj[i] = P / m * powmod(P / m % m, m - 2, m) % P;
    }
    vector<vector<T>> p(sz);
    for (size_t i = 0; i < sz; ++i) {
        p[i] = sum_convolution(arr1, arr2, mods[i]);
    }
    const size_t n = p[0].size();
    vector<T> res(n);
    for (size_t i = 0; i < n; ++i) {
        H sm = 0;
        for (size_t j = 0; j < sz; ++j) {
            sm += p[j][i] * mnoj[j] % P;
        }
        res[i] = sm % P % mod;
    }
    return res;
}
