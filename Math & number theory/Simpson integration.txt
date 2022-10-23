namespace Simpson {
    inline ld approx(ld l, ld r, ld fl, ld fmd, ld fr) {
        return (fl + 4 * fmd + fr) * (r - l);
    }

    ld go(ld s, ld fl, ld fr, ld fmd, ld l, ld r) {
        ld md = (l + r) / 2;
        ld flm = f((l + md) / 2);
        ld frm = f((md + r) / 2);
        ld sl = approx(l, md, fl, flm, fmd);
        ld sr = approx(md, r, fmd, frm, fr);
        if (abs(s - sl - sr) < eps) return sl + sr;
        return go(sl, fl, fmd, flm, l, md) + go(sr, fmd, fr, frm, md, r);
    }

    ld integrate(ld l, ld r) {
        ld md = (l + r) / 2, fl = f(l), fr = f(r), fmd = f(md);
        ld s = approx(l, r, fl, fmd, fr);
        return go(s, fl, fr, fmd, l, r) / 6;
    }
};