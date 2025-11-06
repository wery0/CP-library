//Finds all left shifts of arr1, which equals to arr2.
/*
  arr1 = {1, 2, 1, 2, 1, 2}
  arr2 = {2, 1, 2, 1, 2, 1}
  f(arr1, arr2) = {1, 3, 5}
*/
//O(n)
template<typename T_arr>
vector<int> find_all_left_shifts(const T_arr& arr1, T_arr arr2) {
    const size_t n = arr1.size(); assert(arr2.size() == n);
    arr2.insert(arr2.end(), arr1.begin(), arr1.end());
    arr2.insert(arr2.end(), arr1.begin(), arr1.end());
    vector<int> zf(n * 3), ans;
    for (int i = 1, l = 0, r = 0; i < n * 2; ++i) {
        zf[i] = max(0, min(r - i, zf[i - l]));
        while (zf[i] + i < n * 3 && arr2[zf[i]] == arr2[zf[i] + i]) {
            ++zf[i];
        }
        if (i + zf[i] > r) {
            r = i + zf[i];
            l = i;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (zf[n + i] >= n) ans.push_back(i);
    }
    return ans;
}
