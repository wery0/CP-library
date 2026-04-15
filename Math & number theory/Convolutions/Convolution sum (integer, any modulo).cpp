//Uses CRT theorem to calculate answer by any modulo
//O(|mods| * T(sum_convolution)) 
template<typename T, typename H = __uint128_t>
vector<T> sum_convolution_arbitrary_mod(vector<T>& arr1, vector<T>& arr2, const T mod) {
    assert(numeric_limits<T>::max() / mod >= mod && "mod * mod should fit into T!");
    if (arr1.empty() || arr2.empty()) return {};
    auto powmod = [&](T x, T k, T mod) {T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x % mod; x = x * x % mod;} return o;};
    static constexpr array<T, 3> mods = {167772161, 469762049, 754974721};  //product ~5.95e25. Coefficients of result should fit into this number.
    static const H P = accumulate(mods.begin(), mods.end(), H(1), multiplies<H>());
    static const size_t sz = mods.size();
    vector<H> mnoj(sz);
    for (size_t i = 0; i < sz; ++i) mnoj[i] = P / mods[i] * powmod(P / mods[i] % mods[i], mods[i] - 2, mods[i]) % P;
    vector<vector<T>> p(sz);
    for (size_t i = 0; i < sz; ++i) p[i] = sum_convolution_mod(arr1, arr2, mods[i]);
    const size_t n = p[0].size();
    vector<T> res(n);
    for (size_t i = 0; i < n; ++i) {
        H sm = 0;
        for (size_t j = 0; j < sz; ++j) sm += p[j][i] * mnoj[j] % P;
        res[i] = sm % P % mod;
    }
    return res;
}

//C++20 version with 2 modulos
template<typename T, typename H = __uint128_t>
vector<T> sum_convolution_arbitrary_mod(vector<T>& arr1, vector<T>& arr2, const T mod) {
    if (arr1.empty() || arr2.empty()) return {};
    const size_t res_size = arr1.size() + arr2.size() - 1;
    static constexpr array<int64_t, 2> mods = {1000307949569, 1000391835649};  //product ~1.00e24. Coefficients of result should fit into this number.
    static constexpr H P = accumulate(mods.begin(), mods.end(), H(1), multiplies<H>());
    vector<H> sm(res_size);
    auto crt_step = []<int64_t Mod>(vector<T>& a1, vector<T>& a2, vector<H>& smref, H P0) {
        using mint = MontgomeryMint<int64_t, __int128_t, Mod>;
        const H mnoj = P0 / Mod * inv(mint(P0 / Mod % Mod)).get_val();
        for (size_t i = 0; T val : sum_convolution<T, mint>(a1, a2)) smref[i++] += val * mnoj % P0;
    };
    [&]<size_t... Is>(index_sequence<Is...>) {
        (crt_step.template operator()<mods[Is]>(arr1, arr2, sm, P), ...);
    }(make_index_sequence<mods.size()> {});
    vector<T> res(res_size);
    for (size_t i = 0; i < res_size; ++i) res[i] = sm[i] % P % mod;
    return res;
}
