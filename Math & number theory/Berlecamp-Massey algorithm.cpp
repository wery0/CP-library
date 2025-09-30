
//For a given sequence of numbers finds coefficients of shortest linear recurrence that generates it
//O(n * recurrence_length) <= O(n ^ 2)
template<typename T>
vector<T> berlekamp_massey(const vector<T>& a) {
	const size_t n = a.size();
	if (n == 0) return {};
	vector<T> b(n), c(n);
	T p = b[0] = c[0] = 1;
	int l = 0;
	for (int i = 0, k = 1; i < n; ++i, ++k) {
		T d = a[i];
		for (int j = 1; j <= l; ++j) d += c[j] * a[i - j];
		if (d == 0) continue;
		vector<T> temp = c;
		T coef = d / p;
		for (int j = k; j < n; ++j) c[j] -= coef * b[j - k];
		if (l * 2 <= i) {
			l = i + 1 - l;
			b = temp;
			p = d;
			k = 0;
		}
	}
	c.resize(l + 1);
	c.erase(c.begin());
	for (T& x : c) x = -x;
	return c;
}
