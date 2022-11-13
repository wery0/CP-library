template<typename T_arr>
ll manacher(const T_arr& t) {
    int a = t.size();
    vec<int> odd(a);   //odd("aaaaa")  = {1, 2, 3, 2, 1}
    vec<int> even(a);  //even("aaaaa") = {0, 1, 2, 2, 1}
    for (int q = 0, l = 0, r = 0; q < a; ++q) {
        int& d = odd[q];
        d = q > r ? 1 : max(0, min(r - q, odd[r - q + l]));
        while (q - d >= 0 && q + d < a && t[q - d] == t[q + d]) {
            ++d;
        }
        l = chmax(r, q + d - 1) ? q - d + 1 : l;
    }
    for (int q = 1, l = 0, r = 0; q < a; ++q) {
        int& d = even[q];
        d = q > r ? 0 : max(0, min(r - q, even[r - q + l + 1]));
        while (q - d - 1 >= 0 && q + d < a && t[q - d - 1] == t[q + d]) {
            ++d;
        }
        l = chmax(r, q + d) ? q - d - 1 : l;
    }
    return accumulate(all(odd), 0ll) + accumulate(all(even), 0ll);
}

// bool is_palindrome(int l, int r) {
//     int d = r - l + 1;
//     return d & 1 ? odd[(r + l) / 2] > d / 2 : even[(r + l) / 2 + 1] >= d / 2;
// }