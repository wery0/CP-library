//Calculates # palindromic subarrays
//O(n)
template<typename T_arr, typename C = int64_t>
C manacher(const T_arr& arr) {
    const size_t n = arr.size();
    vector<int> odd(n);   //odd("aaaaa")  = {1, 2, 3, 2, 1}
    vector<int> even(n);  //even("aaaaa") = {0, 1, 2, 2, 1}
    for (int i = 0, l = 0, r = 0; i < n; ++i) {
        int& d = odd[i];
        d = i > r ? 1 : max(0, min(r - i, odd[r - i + l]));
        while (i - d >= 0 && i + d < n && arr[i - d] == arr[i + d]) ++d;
        if (i + d - 1 > r) r = i + d - 1, l = i - d + 1;
    }
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        int& d = even[i];
        d = i > r ? 0 : max(0, min(r - i, even[r - i + l + 1]));
        while (i - d - 1 >= 0 && i + d < n && arr[i - d - 1] == arr[i + d]) ++d;
        if (i + d > r) r = i + d, l = i - d - 1;
    }
    return accumulate(odd.begin(), odd.end(), (C)0) + accumulate(even.begin(), even.end(), (C)0);
}

// bool is_palindrome(int l, int r) {
//     int d = r - l + 1;
//     return d & 1 ? odd[(r + l) / 2] > d / 2 : even[(r + l) / 2 + 1] >= d / 2;
// }
