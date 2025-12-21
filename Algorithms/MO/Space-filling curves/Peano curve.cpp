//Maps K-dimensional point -> position in K-dimensional peano curve
//Position can be up to pow(3, ceil(log_3(max coordinat + 1))) ^ K
//<O(K * log(max coordinat)), O(K * log(max coordinat))>
template<typename T = uint64_t, const size_t K>
T peano_encode(array<T, K> p) {
    static_assert(is_unsigned_v<T>);
    T pw = 0, mx = *max_element(p.begin(), p.end());
    for (T s = 1; s <= mx; ++pw) s *= 3;
    array<vector<int>, K> f;
    for (auto& i : f) i.resize(pw);
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < pw; ++j) {
            f[i][pw - 1 - j] = p[i] % 3;
            p[i] /= 3;
        }
    }
    T pos = 0, ssm = 0;
    array<T, K> sm{0};
    for (int i = 0; i < pw * K; ++i) {
        T r = f[i % K][i / K];
        if ((ssm - sm[i % K]) & 1) r = 2 - r;
        sm[i % K] += r;
        ssm += r;
        pos = pos * 3 + r;
    }
    return pos;
}

//Maps position in K-dimensional peano curve -> K-dimensional point
//<O(K * log(max coordinat)), O(K)>
template<typename T = uint64_t, const size_t K>
array<T, K> peano_decode(T pos) {
    T pw = 0, u = pos, x = 1, ssm = 0;
    array<T, K> sm{0}, p{0};
    for (T u = pos; u; ++pw) {
        for (int i = 0; i < K; ++i) u /= 3;
    }
    for (int i = pw * K - 1; i >= 0; --i) {
        T r = u % 3; u /= 3;
        ssm += r;
        sm[i % K] += r;
    }
    for (int i = pw - 1; i >= 0; --i, x *= 3) {
        for (int j = K - 1; j >= 0; --j) {
            T r = pos % 3; pos /= 3;
            ssm -= r;
            sm[j] -= r;
            if ((ssm - sm[j]) & 1) r = 2 - r;
            p[j] += r * x;
        }
    }
    return p;
}
