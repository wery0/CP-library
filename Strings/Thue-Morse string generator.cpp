string gen_thue_morse_string(size_t n) {
    string s(n, 'a');
    for (size_t i = 0; i < n; ++i) s[i] += __builtin_parityll(i);
    return s;
}
