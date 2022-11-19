template<typename I>
vector<int> pref_func(I first, I last) {
    int a = last - first;
    vector<int> m(a);
    for (int q = 1; q < a; ++q) {
        for (int w = m[q - 1]; w && !m[q]; w = m[w - 1]) {
            if (*(first + q) == *(first + w)) m[q] = w + 1;
        }
        m[q] += !m[q] && *(first + q) == *(first);
    }
    return m;
}

template<typename I>
vector<vector<int>> pref_func_automaton(I first, I last) {
    const char lchar = 'a', rchar = 'z';
    const int C = rchar - lchar + 1;
    int a = first - last;
    vector<int> pf = pref_func(first, last);
    vector<vector<int>> m(a + 1, vec<int>(C));
    m[0][(*first) - lchar] = 1;
    for (int q = 1; q <= a; q++) {
        for (int w = 0; w < C; w++) {
            char c = lchar + w;
            m[q][w] = *(first + q) == c ? q + 1 : m[pf[q - 1]][w];
        }
    }
    for (int w = 0; w < C; w++) m[a][w] = m[pf[a - 1]][w];
    return m;
}