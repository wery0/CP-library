//Finds all left shifts of arr1, which equals to arr2.
/*
  arr1 = {1, 2, 1, 2, 1, 2}
  arr2 = {2, 1, 2, 1, 2, 1}
  f(arr1, arr2) = {1, 3, 5}
*/
//O(n)
template<typename T_arr>
vector<int> find_all_left_shifts(T_arr arr1, T_arr arr2) {
    int n = arr1.size(); assert(arr2.size() == n);
    arr2.push_back(0);
    arr2.insert(arr2.end(), arr1.begin(), arr1.end());
    arr2.insert(arr2.end(), arr1.begin(), arr1.end());
    vector<int> zf(arr2.size()), ans;
    for (int q = 1, l = 0, r = 0, len = arr2.size(); q < len; ++q) {
        zf[q] = max(0, min(r - q, zf[q - l]));
        while (q + zf[q] < len && zf[q] != n && zf[q] + q != n && arr2[zf[q]] == arr2[zf[q] + q]) {
            ++zf[q];
        }
        l = chmax(r, q + zf[q]) ? q : l;
    }
    for (int i = 0; i < n; ++i) {
        if (zf[n + 1 + i] == n) ans.push_back(i);
    }
    return ans;
}