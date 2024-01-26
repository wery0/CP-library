template<typename T>
vector<T> arbitrary_mod_mul(vector<T>& arr1, vector<T>& arr2, const T mod) {
    auto powmod = [&](int64_t x, int64_t k, int64_t mod) {
        int64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    auto modinv = [&](int64_t x, int64_t mod) {return powmod(x, mod - 2, mod);};
    const vector<int> mods = {167772161, 469762049, 754974721}; //product ~5.95e25. Coefficients of result should fit into this number.
    const size_t sz = mods.size();
    const __uint128_t P = accumulate(mods.begin(), mods.end(), (__uint128_t)1, multiplies<__uint128_t>());
    vector<__uint128_t> mnoj(sz);
    for (size_t i = 0; i < sz; ++i) {
        mnoj[i] = P / mods[i] * modinv(P / mods[i] % mods[i], mods[i]) % P;
    }
    vector<vector<T>> p(sz);
    for (size_t i = 0; i < sz; ++i) {
        p[i] = mul(arr1, arr2, mods[i]);
    }
    size_t n = p[0].size();
    vector<T> ans(n);
    for (size_t i = 0; i < n; ++i) {
        __uint128_t sm = 0;
        for (size_t j = 0; j < sz; ++j) {
            sm += p[j][i] * mnoj[j] % P;
        }
        ans[i] = sm % P % mod;
    }
    return ans;
}
