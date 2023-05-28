/*
Generates array A of distinct numbers s. t. 1 <= A[i] <= N and A[i] + A[j] != A[k] + A[l]
for i != j, k != l, (i, j) != (k, l) (i. e. all pairwise sums are different)
*/
//A.size() approaches sqrt(N / 2) as N -> inf
//It uses the fact that for any prime number P the set {i * 2P + (i^2+1)%(2P) | 0 <= i <= P} is good.
template<typename T>
vector<T> gen_array_with_distinct_pairwise_sums(T N) {
    static_assert(is_integral_v<T>);
    if (N < 10) return N < 1 ? vector<T>() : N == 1 ? vector<T>({1}) : N == 2 ? vector<T>({1, 2}) : vector<T>({1, 2, 3});
    static const vector<int> Ns = {26, 44, 68, 104, 186, 254, 352, 429, 596};
    static const vector<int> Bs = {3, 5, 7, 9, 11, 17, 22, 26, 29};
    static const vector<int> Ss = {4, 6, 7, 8, 10, 11, 12, 14, 15};
    T B, sz;
    if (N < Ns.back()) {
        int i = 0;
        while (N >= Ns[i]) ++i;
        B = Bs[i], sz = Ss[i];
    } else {
        B = (T)(sqrtl(2 * N - 1.75) - 0.5) / 2;
        while (!is_prime(B)) --B;                //Use any prime checking method
        sz = B + 1;
        B *= 2;
    }
    vector<T> ans(sz);
    for (size_t q = 0; q < sz; ++q) {
        ans[q] = B * q + (q * q + 1) % B;
    }
    return ans;
}
