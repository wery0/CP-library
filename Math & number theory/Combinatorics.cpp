template<typename T = int64_t>
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

const int64_t mod = 1e9 + 7;
const int64_t G = 3e5 + 5;
vector<int64_t> fc, ifc;

int64_t powmod(int64_t x, int64_t k) {
    if (k < 0) return 0;
    int64_t o = 1;
    for (; k; k >>= 1) {
        if (k & 1) o = o * x % mod;
        x = x * x % mod;
    }
    return o;
}

int64_t inv(int64_t x) {return powmod(x, mod - 2);}

int64_t C(int64_t n, int64_t k) {return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[k] % mod * ifc[n - k] % mod;}

int main() {
    fast;
    fc.resize(G, 1);
    for (int i = 1; i < G; ++i) fc[i] = fc[i - 1] * i % mod;
    ifc.resize(G, inv(fc[G - 1]));
    for (int i = G - 2; i >= 0; --i) ifc[i] = ifc[i + 1] * (i + 1) % mod;
}
