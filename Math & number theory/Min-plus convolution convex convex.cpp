//Returns vector dp, s. t. dp[k] = min{0 <= i <= k}(m1[i] + m2[k - i])
//m1, m2 must both be convex arrays!
//O(n)
template<typename T>
vector<T> min_plus_convolution_convex_convex(const vector<T>& m1, const vector<T>& m2) {
	if (m1.empty() || m2.empty()) return m1.empty() ? m2 : m1;
	const size_t n1 = m1.size();
	const size_t n2 = m2.size();
	for (size_t i = 1; i + 1 < n1; ++i) assert(m1[i] - m1[i - 1] <= m1[i + 1] - m1[i] && "m1 must be convex!");
	for (size_t i = 1; i + 1 < n2; ++i) assert(m2[i] - m2[i - 1] <= m2[i + 1] - m2[i] && "m2 must be convex!");
	vector<T> dp(n1 + n2 - 1);
	size_t i1 = 0, i2 = 0;
	dp[0] = m1[0] + m2[0];
	for (size_t i = 1; i < n1 + n2 - 1; ++i) {
		if (i2 == n2 - 1 || (i1 != n1 - 1 && m1[i1 + 1] + m2[i2] < m1[i1] + m2[i2 + 1])) {
			dp[i] = m1[++i1] + m2[i2];
		} else {
			dp[i] = m1[i1] + m2[++i2];
		}
	}
	return dp;
}