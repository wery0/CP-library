template<typename Arr, typename T = int64_t>
T manacher(const Arr& t) {
    size_t n = t.size();
    vector<int> odd(n);   //odd("aaaaa")  = {1, 2, 3, 2, 1}
    vector<int> even(n);  //even("aaaaa") = {0, 1, 2, 2, 1}
    for (int i = 0, l = 0, r = 0; i < n; ++i) {
        int& d = odd[i];
        d = i > r ? 1 : max(0, min(r - i, odd[r - i + l]));
        while (i - d >= 0 && i + d < n && t[i - d] == t[i + d]) ++d;
        if (i + d - 1 > r) r = i + d - 1, l = i - d + 1;
    }
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        int& d = even[i];
        d = i > r ? 0 : max(0, min(r - i, even[r - i + l + 1]));
        while (i - d - 1 >= 0 && i + d < n && t[i - d - 1] == t[i + d]) ++d;
        if (i + d > r) r = i + d, l = i - d - 1;
    }
    return accumulate(odd.begin(), odd.end(), (T)0) + accumulate(even.begin(), even.end(), (T)0);
}

// bool is_palindrome(int l, int r) {
//     int d = r - l + 1;
//     return d & 1 ? odd[(r + l) / 2] > d / 2 : even[(r + l) / 2 + 1] >= d / 2;
// }