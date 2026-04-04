//a[i] = i < n ? a[i] : sum(c[j] * a[i - 1 - j])
//Complexity: O(O(sum_convolution) * log(k))
template<typename T>
T kth_value_of_recurrence_relation_bostan_mori(vector<T> a, vector<T> c, uint64_t k) {
    c.insert(c.begin(), T(-1));
    vector<T> st = sum_convolution(a, c);
    if (st.size() + 1 > c.size()) st.resize(c.size() - 1);
    return bostan_mori(st, c, k);
}
