template<typename T>
T extended_euclid_inv(T a, T mod) {
	static_assert(is_signed_v<T>);
	auto gcd = [](auto&& gcd, T a, T b, T& x, T& y) {
		if (!a) {
	        x = 0, y = 1;
	        return b;
	    }
	    T g = gcd(gcd, b % a, a, x, y);
	    T nx = y - x * (b / a), ny = x;
	    x = nx, y = ny;
	    return g;
	};
    T x, y, g = gcd(gcd, a, mod, x, y);
    if (g != 1) return -1;
    if (x < 0) x += (-x + mod - 1) / mod * mod;
    x %= mod;
    return x;
}

template<typename T, const T mod>
struct comb_mod {
    static_assert(is_integral_v<T>);
    static_assert(mod > 0);
    static_assert(numeric_limits<T>::max() / mod > mod && "Product of two numbers < mod should fit into T!");

    size_t N;
    vector<T> fc, ifc;

    comb_mod() = default;
    comb_mod(size_t N): N(N), fc(N, 1), ifc(N, 1) {
        assert(N > 1);
        for (size_t i = 1; i < N; ++i) fc[i] = fc[i - 1] * i % mod;
        ifc[N - 1] = inv(fc[N - 1]);
        for (size_t i = N - 2; i; --i) ifc[i] = ifc[i + 1] * (i + 1);
    }

    T inv(T x) {
        return binpow(x, mod - 2);
    }

    T binpow(T x, int64_t k) {
        assert(k >= 0);
        T res = 1;
        for (; k; k >>= 1) {
            if (k & 1) res = res * x % mod;
            x = x * x % mod;
        }
        return res;
    }

    T A(int64_t n, int64_t k) {assert(n < N); return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[n - k] % mod;}
    T C(int64_t n, int64_t k) {assert(n < N); return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[k] % mod * ifc[n - k] % mod;}
};

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
};
