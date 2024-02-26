//Sum convolution (aka polynomial product, fft, ntt)
//res[k] = sum({arr1[i] * arr2[j] | i + j == k})
//mod must be prime and (mod - 1) should be divisible on 2^k, s. t. n <= 2^k.
//For example 998244353 is good for n <= 2 ^ 23, since 998244352 = 7 * 17 * 2 ^ 23
//O(nlog(n))
template<typename T>
vector<T> sum_convolution_mod(vector<T> arr1, vector<T> arr2, const T mod) {
    static_assert(is_integral_v<T>);
    assert(numeric_limits<T>::max() / mod >= mod && "mod * mod should fit into T!");
    if (arr1.empty() || arr2.empty()) return {};
    auto binpow = [&](T x, T k) {T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x % mod; x = x * x % mod;} return o;};
    const size_t res_size = arr1.size() + arr2.size() - 1;
    const size_t m = max(arr1.size(), arr2.size()) * 2;
    const size_t n = m & (m - 1) ? 2 << __lg(m) : m;
    assert(n <= (1ull << __builtin_ctzll(mod - 1)) && "(mod - 1) should be divisible on 2 ^ k, s. t. n <= 2 ^ k");
    arr1.resize(n);
    arr2.resize(n);
    T g = 0;
    for (T i = 1; i < mod && !g; ++i) {
        if (binpow(i, (mod - 1) / 2) == mod - 1) {
            g = binpow(i, (mod - 1) / n);
        }
    }
    assert(g);
    auto perm = [&](auto&& perm, vector<T>& m, size_t l, size_t r, size_t d, size_t s) {
        if (l == r) {
            if (l < d) swap(m[l], m[d]);
            return;
        }
        size_t md = (l + r) >> 1;
        perm(perm, m, l, md, d, s << 1);
        perm(perm, m, md + 1, r, d + s, s << 1);
    };
    vector<T> pw(n, (T)1);
    auto fft = [&](vector<T>& m, T t) {
        perm(perm, m, 0, n - 1, 0, 1);
        for (size_t d = 1; d * 2 <= n; d <<= 1) {
            const T base = binpow(t, n / (d * 2));
            for (size_t i = 1; i < d; ++i) pw[i] = pw[i - 1] * base % mod;
            for (size_t l = 0; l < n; l += d) {
                for (size_t i = 0; i < d; ++i, ++l) {
                    const T u = pw[i] * m[l + d] % mod;
                    m[l + d] = m[l] + mod - u;
                    m[l] += u;
                }
            }
            for (T& val : m) val -= val < mod ? 0 : mod;
        }
    };
    fft(arr1, g);
    fft(arr2, g);
    for (size_t i = 0; i < n; ++i) arr1[i] = arr1[i] * arr2[i] % mod;
    fft(arr1, binpow(g, mod - 2));
    arr1.resize(res_size);
    const T in = binpow(n, mod - 2);
    for (T& val : arr1) val = val * in % mod;
    return arr1;
}

//Specialization for modulo arithmetic types
template<typename mint>
vector<mint> sum_convolution(vector<mint> arr1, vector<mint> arr2) {
    if (arr1.empty() || arr2.empty()) return {};
    static constexpr auto mod = mint::get_mod();
    const size_t res_size = arr1.size() + arr2.size() - 1;
    const size_t m = max(arr1.size(), arr2.size()) * 2;
    const size_t n = m & (m - 1) ? 2 << __lg(m) : m;
    assert(n <= (1ull << __builtin_ctzll(mod - 1)) && "(mod - 1) should be divisible on 2 ^ k, s. t. n <= 2 ^ k");
    arr1.resize(n);
    arr2.resize(n);
    mint generator = 0;
    for (mint i = mod - 1; i; --i) {
        if (i.pow((mod - 1) / 2) == mod - 1) {
            generator = i;
            break;
        }
    }
    assert(generator);
    generator = generator.pow((mod - 1) / n);
    auto perm = [&](auto&& perm, vector<mint>& m, size_t l, size_t r, size_t d, size_t s) {
        if (l == r) {
            if (l < d) swap(m[l], m[d]);
            return;
        }
        size_t md = (l + r) >> 1;
        perm(perm, m, l, md, d, s << 1);
        perm(perm, m, md + 1, r, d + s, s << 1);
    };
    vector pw(n, (mint)1);
    auto fft = [&](vector<mint>& m, mint t) {
        perm(perm, m, 0, n - 1, 0, 1);
        for (size_t d = 1; d * 2 <= n; d <<= 1) {
            const mint base = t.pow(n / (d * 2));
            for (size_t i = 1; i < d; ++i) pw[i] = pw[i - 1] * base;
            for (size_t l = 0; l < n; l += d) {
                for (size_t i = 0; i < d; ++i, ++l) {
                    const mint u = pw[i] * m[l + d];
                    m[l + d] = m[l] - u;
                    m[l] += u;
                }
            }
        }
    };
    fft(arr1, generator);
    fft(arr2, generator);
    for (size_t i = 0; i < n; ++i) arr1[i] *= arr2[i];
    fft(arr1, generator.inv());
    arr1.resize(res_size);
    const mint in = mint(n).inv();
    for (mint& val : arr1) val *= in;
    return arr1;
}
