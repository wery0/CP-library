//zf[i] = max d, s. t. substr(0, d) == substr(i, d)
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
