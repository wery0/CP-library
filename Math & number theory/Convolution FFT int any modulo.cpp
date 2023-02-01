template<typename T>
vector<T> arbitrary_mod_mul(vector<T>& arr1, vector<T>& arr2, const ll mod) {
    auto powmod = [&](ll x, ll k, ll mod) {
        ll o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % mod;
            x = x * x % mod;
        }
        return o;
    };
    auto modinv = [&](ll x, ll mod) {return powmod(x, mod - 2, mod);};
    const vector<int> mods = {167772161, 469762049, 754974721}; //product ~5.95e25. Coefficients of result should fit into this number.
    int sz = mods.size();
    const __uint128_t P = accumulate(mods.begin(), mods.end(), (__uint128_t)1, multiplies<__uint128_t>());
    vector<__uint128_t> mnoj(sz);
    for (int q = 0; q < sz; ++q) {
        mnoj[q] = P / mods[q] * modinv(P / mods[q] % mods[q], mods[q]) % P;
    }
    vector<vector<T>> p(sz);
    for (int q = 0; q < sz; ++q) {
        p[q] = mul(arr1, arr2, mods[q]);
    }
    int a = p[0].size();
    vector<T> ans(a);
    for (int q = 0; q < a; ++q) {
        __uint128_t sm = 0;
        for (int w = 0; w < sz; ++w) {
            sm += p[w][q] * mnoj[w] % P;
        }
        ans[q] = sm % P % mod;
    }
    return ans;
}