template<typename T = long double>
T simpson_integration(const T l, const T r, const T EPS, auto f) {
    auto approx = [](T l, T r, T fl, T fmd, T fr) {
        return (fl + 4 * fmd + fr) * (r - l);
    };
    auto go = [](auto&& go, T s, T fl, T fr, T fmd, T l, T r) {
        T md = (l + r) / 2;
        T flm = f((l + md) / 2);
        T frm = f((md + r) / 2);
        T sl = approx(l, md, fl, flm, fmd);
        T sr = approx(md, r, fmd, frm, fr);
        if (abs(s - sl - sr) < EPS) return sl + sr;
        return go(go, sl, fl, fmd, flm, l, md) + go(go, sr, fmd, fr, frm, md, r);
    };
    T fl = f(l), fr = f(r), fmd = f((l + r) / 2);
    T s = approx(l, r, fl, fmd, fr);
    return go(go, s, fl, fr, fmd, l, r) / 6;
}
