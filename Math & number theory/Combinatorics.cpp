template<typename T>
struct comb {
    size_t N;
    vector<T> fc, ifc;

    comb() = default;
    comb(size_t N): N(N), fc(N, 1), ifc(N, 1) {
        assert(N > 1);
        for (size_t i = 1; i < N; ++i) fc[i] = fc[i - 1] * i;
        ifc[N - 1] = inv(fc[N - 1]);
        for (size_t i = N - 2; i; --i) ifc[i] = ifc[i + 1] * (i + 1);
    }

    T binpow(T x, int64_t k) {
        assert(k >= 0);
        T res = 1;
        for (; k; k >>= 1) {
            if (k & 1) res *= x;
            x *= x;
        }
        return res;
    }

    T A(int64_t n, int64_t k) {assert(n < N); return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[n - k];}
    T C(int64_t n, int64_t k) {assert(n < N); return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[k] * ifc[n - k];}
    T C_bigN_smallK(int64_t n, int64_t k) {if (n < 0 || k < 0 || k > n) return 0; k = min(k, n - k); assert(k < N); T res = 1, x = n; for (int i = 0; i < k; ++i, --x) res *= x; return res * ifc[k];}
};
