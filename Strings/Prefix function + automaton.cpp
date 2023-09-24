//pf[i] = max d < i, s. t. substr(0, d) == substr(i - d, d)
//O(n)
template<typename I>
vector<int> prefix_function(I first, I last) {
    const size_t n = last - first;
    vector<int> pf(n);
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = pf[i - 1]; j && !pf[i]; j = pf[j - 1]) {
            if (*(first + i) == *(first + j)) pf[i] = j + 1;
        }
        pf[i] += !pf[i] && *(first + i) == *(first);
    }
    return pf;
}

//m[i][j] = new value of PF after appending char j to the end of the sequence with PF i
//O(n * ALPHABET)
template<typename I>
vector<vector<int>> prefix_function_automaton(I first, I last) {
    //Change, if need
    const size_t ALPHABET = 26;
    const char FIRST_CHAR = 'a';

    const size_t n = last - first;
    vector<int> pf = prefix_function(first, last);
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
