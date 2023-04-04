//long double f(long double x) {return sin(x) * x;}
//const long double EPS = 1e-9; 
namespace Simpson {

    using T = long double;

    T approx(T l, T r, T fl, T fmd, T fr) {
        return (fl + 4 * fmd + fr) * (r - l);
    }

    T go(T s, T fl, T fr, T fmd, T l, T r) {
        T md = (l + r) / 2;
        T flm = f((l + md) / 2);
        T frm = f((md + r) / 2);
        T sl = approx(l, md, fl, flm, fmd);
        T sr = approx(md, r, fmd, frm, fr);
        if (abs(s - sl - sr) < EPS) return sl + sr;
        return go(sl, fl, fmd, flm, l, md) + go(sr, fmd, fr, frm, md, r);
    }

    T integrate(T l, T r) {
        T md = (l + r) / 2, fl = f(l), fr = f(r), fmd = f(md);
        T s = approx(l, r, fl, fmd, fr);
        return go(s, fl, fr, fmd, l, r) / 6;
    }
};
