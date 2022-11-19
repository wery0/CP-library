template<typename I>
vector<int> Z(I first, I last) {
    int a = last - first;
    vector<int> m(a);
    for (int q = 1, l = 0, r = 0; q < a; ++q) {
        m[q] = max(0, min(r - q, m[q - l]));
        while (q + m[q] < a && *(first + m[q]) == *(first + m[q] + q)) {
            ++m[q];
        }
        l = chmax(r, q + m[q]) ? q : l;
    }
    return m;
}