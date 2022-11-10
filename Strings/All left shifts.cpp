template<typename I>
vector<int> Z(I first, I last, int ban) {
    int a = last - first;
    vector<int> m(a);
    for (int q = 1, l = 0, r = 0; q < a; ++q) {
        m[q] = max(0, min(r - q, m[q - l]));
        while (q + m[q] < a && m[q] != ban && m[q] + q != ban && *(first + m[q]) == *(first + m[q] + q)) {
            ++m[q];
        }
        l = chmax(r, q + m[q]) ? q : l;
    }
    return m;
}

//Finds all left shifts of m, which equals to n.
template<typename T_arr>
vector<int> find_all_left_shifts(T_arr m, T_arr n) {
    int a = m.size(); assert(n.size() == a);
    n.push_back(0);
    n.insert(n.end(), m.begin(), m.end());
    n.insert(n.end(), m.begin(), m.end());
    vector<int> zf = Z(n.begin(), n.end(), a), ans;
    for (int q = a + 1, i = 0; i < a; ++q, ++i) {
        if (zf[q] == a) ans.push_back(i);
    }
    return ans;
}