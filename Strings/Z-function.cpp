//zf[i] = max d, s. t. substr(0, d) == substr(i, d), zf[0] = 0
template<typename I>
vector<int> Z(I first, I last) {
    const size_t n = last - first;
    vector<int> zf(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        zf[i] = max(0, min(r - i, zf	[i - l]));
        while (i + zf[i] < n && *(first + zf[i]) == *(first + zf[i] + i)) {
            ++zf[i];
        }
        if (i + zf[i] > r) {
            r = i + zf[i];
            l = i;
        }
    }
    return zf;
}

//Transforms Z-function to prefix-function
vector<int> ZtoP(const vector<int>& z) {
    const size_t n = z.size();
    vector<int> p(n);
    auto chmax = [](int& x, int y) {x = max(x, y);};
    for (size_t i = 1; i < n; ++i) chmax(p[i + z[i] - 1], z[i]);
    for (size_t i = n ? n - 1 : 0; i >= 1; --i) chmax(p[i - 1], p[i] - 1);
    return p;
}
