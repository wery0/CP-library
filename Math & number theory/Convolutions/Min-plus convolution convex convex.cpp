//res[k] = min{0 <= i <= k}(arr1[i] + arr2[k - i])
//arr1, arr2 must both be convex arrays!
//O(n)
template<typename T>
vector<T> min_plus_convolution_convex_convex(const vector<T>& arr1, const vector<T>& arr2) {
	if (arr1.empty() || arr2.empty()) return arr1.empty() ? arr2 : arr1;
	const size_t n1 = arr1.size();
	const size_t n2 = arr2.size();
	for (size_t i = 1; i + 1 < n1; ++i) assert(arr1[i] - arr1[i - 1] <= arr1[i + 1] - arr1[i] && "arr1 must be convex!");
	for (size_t i = 1; i + 1 < n2; ++i) assert(arr2[i] - arr2[i - 1] <= arr2[i + 1] - arr2[i] && "arr2 must be convex!");
	vector<T> res(n1 + n2 - 1);
	size_t i1 = 0, i2 = 0;
	res[0] = arr1[0] + arr2[0];
	for (size_t i = 1; i < n1 + n2 - 1; ++i) {
		if (i2 == n2 - 1 || (i1 != n1 - 1 && arr1[i1 + 1] + arr2[i2] < arr1[i1] + arr2[i2 + 1])) {
			res[i] = arr1[++i1] + arr2[i2];
		} else {
			res[i] = arr1[i1] + arr2[++i2];
		}
	}
	return res;
}
