//Finds lex min cyclic shift of array
//Usage: std::rotate(s.begin(), s.begin() + min_rotation(s), s.end())
//O(n)
template<typename T_arr>
int min_rotation(T_arr s) {
    const size_t n = s.size();
    size_t o = 0;
    s.insert(s.end(), s.begin(), s.end());
    for (size_t b = 0; b < n; ++b) {
        for (size_t k = 0; k < n; ++k) {
            if (o + k == b || s[o + k] < s[b + k]) {b += max<ssize_t>(0, k - 1); break;}
            if (s[b + k] < s[o + k]) {o = b; break;}
        }
    }
    return o;
}
