str gen_thue_morse_string(int a) {
    str s = string(a, 'a');
    for (int i = 0; i < a; ++i) s[i] += __builtin_parity(i);
    return s;
}