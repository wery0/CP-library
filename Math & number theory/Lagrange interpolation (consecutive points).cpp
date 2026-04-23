//For polynomial f of degree n calculates f(x) by knowing that f(i) = y[i] for 0 <= i <= n
//Complexity: <O(n), O(n)>
template<typename T>
T lagrange_interpolation_consecutive_points(const vector<T>& y, int64_t x) {
    assert(y.size());
    if (0 <= x && x < y.size()) return y[x];
    const int d = y.size() - 1;
    static vector<T> ifc(1, 1);
    if (int sz = ifc.size(); sz < d + 1) {
        ifc.resize(d + 1);
        T lst = ifc[sz - 1].inv();
        for (int i = sz; i <= d; ++i) lst *= i, ifc[i] = lst;
        ifc[d] = ifc[d].inv();
        for (int i = d - 1; i >= sz; --i) ifc[i] = ifc[i + 1] * (i + 1);
    }
    vector<T> suf(d + 2, 1);
    for (int i = d; i >= 0; --i) suf[i] = suf[i + 1] * (x - i);
    T ans = 0, pref = 1;
    for (int i = 0; i <= d; pref *= x - i, ++i) {
        T num = pref * suf[i + 1];
        T den = ifc[i] * ifc[d - i];
        (d ^ i) & 1 ? (ans -= y[i] * num * den) : (ans += y[i] * num * den);
    }
    return ans;
}
