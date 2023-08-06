//O(n)
template<typename I>
vector<int> pref_func(I first, I last) {
    const size_t n = last - first;
    vector<int> m(n);
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = m[i - 1]; j && !m[i]; j = m[j - 1]) {
            if (*(first + i) == *(first + j)) m[i] = j + 1;
        }
        m[i] += !m[i] && *(first + i) == *(first);
    }
    return m;
}

//O(n * ALPHABET)
template<typename I>
vector<vector<int>> pref_func_automaton(I first, I last) {
    //Change, if need
    const size_t ALPHABET = 26;
    const char FIRST_CHAR = 'a';

    const size_t n = last - first;
    vector<int> pf = pref_func(first, last);
    vector<vector<int>> m(n + 1, vector<int>(ALPHABET));
    m[0][(*first) - FIRST_CHAR] = 1;
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 0; j < ALPHABET; ++j) {
            char c = FIRST_CHAR + j;
            m[i][j] = *(first + i) == c ? i + 1 : m[pf[i - 1]][j];
        }
    }
    for (size_t j = 0; j < ALPHABET; ++j) m[n][j] = m[pf[n - 1]][j];
    return m;
}