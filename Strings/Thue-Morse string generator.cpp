string gen_thue_morse_string(int n) {
    string s(n, 'a');
    for (int i = 0; i < n; ++i) s[i] += __builtin_parity(i);
    return s;
}