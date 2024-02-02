//Multiplication convolution
//res[k] = sum({arr1[i] * arr2[j] | (i * j) % mod == k}), mod should be prime
//O(T(sum_convolution))
template<typename T>
vector<T> mul_convolution(vector<T> arr1, vector<T> arr2, const T mod) {
    assert(numeric_limits<T>::max() / mod >= mod && "mod * mod should fit into T!");
    const size_t n = arr1.size();
    assert(arr2.size() == n);
    auto primitive_root = [&](T n) {
        auto binpow = [&](T x, T k) {T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x % n; x = x * x % n;} return o;};
        set<T> sfc;
        for (T phi = n - 1, i = 2; i <= phi; ++i) {
            while (phi % i == 0) sfc.insert(i), phi /= i;
        }
        for (T g = 1; g < n; ++g) {
            int fl = 1;
            for (T p : sfc) fl &= binpow(g, (n - 1) / p) != 1;
            if (fl) return g;
        }
        return n;
    };
    T g = primitive_root(n);
    vector<T> lg(n, -1), ilg(n, -1);
    for (T cur = 1, i = 0; i + 1 < n; cur = cur * g % n, ++i) {
        lg[cur] = i;
        ilg[i] = cur;
    }
    vector<T> res(n);
    res[0] = (accumulate(arr1.begin(), arr1.end(), (T)0) % mod * arr2[0] +
             accumulate(arr2.begin(), arr2.end(), (T)0) % mod * arr1[0] +
             mod - arr1[0] * arr2[0] % mod) % mod;
    auto transform = [&](vector<T> arr) {
        vector<T> res(n);
        for (size_t i = 1; i < n; ++i) res[lg[i]] = arr[i];
        return res;
    };
    auto a12 = sum_convolution<ll>(transform(arr1), transform(arr2), mod);
    for (size_t i = 0; i < n - 1; ++i) {
        a12[i] += a12[i + n - 1];
        a12[i] -= a12[i] < mod ? 0 : mod;
        res[ilg[i]] = a12[i];
    }
    return res;
}
