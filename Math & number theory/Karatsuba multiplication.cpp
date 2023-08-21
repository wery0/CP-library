//Calculates convolution of two arrays of equal length n
//O(n ^ log_2(3)) ~ O(n ^ 1.585) with super low constant
template<typename T>
void karatsuba(size_t n, const T* __restrict A, const T* __restrict B, T* __restrict res){
    if (n <= 64){
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                res[i + j] += A[i] * B[j];
            }
        }
        return;
    }
    const size_t md = n >> 1;
    alignas(64) T sma[n], E[n];
    memset(E, 0, sizeof(T) * n);
    const auto smb = sma + md;
    for (size_t i = 0; i < md; ++i) {
        sma[i] = A[i] + A[i + md];
        smb[i] = B[i] + B[i + md];
    }
    karatsuba(md, sma, smb, E);
    karatsuba(md, A, B, res);
    karatsuba(md, A + md, B + md, res + n);
    for (size_t i = 0; i < md; ++i) {
        const auto tmp = res[i + md];
        res[i + md] += E[i] - res[i] - res[i + md * 2];
        res[i + md * 2] += E[i + md] - tmp - res[i + md * 3];
    }
}

//Specialization for sizes, known at compilation time
template<size_t n, typename T>
void karatsuba_const(const T* __restrict A, const T* __restrict B, T* __restrict res) {
    if (n <= 32) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                res[i + j] += A[i] * B[j];
            }
        }
        return;
    }
    const size_t md = n >> 1;
    T E[n] = {}, sma[n];
    auto smb = sma + md;
    for (size_t i = 0; i < md; ++i) {
        sma[i] = A[i] + A[i + md];
        smb[i] = B[i] + B[i + md];
    }
    karatsuba_const<md>(sma, smb, E);
    karatsuba_const<md>(A, B, res);
    karatsuba_const<md>(A + md, B + md, res + md * 2);
    for (size_t i = 0; i < md; ++i) {
        const auto tmp = res[i + md];
        res[i + md] += E[i] - res[i] - res[i + md * 2];
        res[i + md * 2] += E[i + md] - tmp - res[i + md * 3];
    }
}
