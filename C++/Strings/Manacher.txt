template<typename T_arr>
ll manacher(T_arr& t) {
    int a = t.size();
    vec<int> odd(a), even(a);  //even("aa") = {0, 1}
    for (int q = 0, l = 0, r = 0; q < a; ++q) {
        int& d = odd[q];
        d = max(0, min(r - q, odd[r - q + l]));
        while (q - d >= 0 && q + d < a && t[q - d] == t[q + d]) {
            ++d;
        }
        l = chmax(r, q + d - 1) ? q - d + 1 : l;
    }
    for (int q = 1, l = 0, r = 0; q < a; ++q) {
        int& d = even[q];
        d = max(0, min(r - q, even[r - q + l + 1]));
        while (q - d - 1 >= 0 && q + d < a && t[q - d - 1] == t[q + d]) {
            ++d;
        }
        l = chmax(r, q + d) ? q - d - 1 : l;
    }
    return accumulate(all(odd), 0ll) + accumulate(all(even), 0ll);
}