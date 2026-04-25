template<typename T>
T extended_euclid_inv(T a, T mod) {
	static_assert(is_signed_v<T>);
	function<T(T, T, T&, T&)> gcd = [&](T a, T b, T& x, T& y) {
		if (!a) {
	        x = 0, y = 1;
	        return b;
	    }
	    T g = gcd(b % a, a, x, y);
	    T nx = y - x * (b / a), ny = x;
	    x = nx, y = ny;
	    return g;
	};
    T x, y, g = gcd(a, mod, x, y);
    if (g != 1) return -1;
    if (x < 0) x += (-x + mod - 1) / mod * mod;
    x %= mod;
    return x;
}
