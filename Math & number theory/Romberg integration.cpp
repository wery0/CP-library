ld romberg(ld l, ld r) {
    vec<ld> t;
    ld d = r - l;
    t.pb(d * (f(l) + f(r)) / 2);
    for (ll k = 1, i = 1; ; k *= 2, d /= 2, ++i) {
        ld last = t.back(), x = l + d / 2, cur = 0;
        for (int j = 0; j < k; j++) cur += f(x), x += d;
        cur = (t[0] + d * cur) / 2;
        ld k1 = 4.0 / 3.0, k2 = 1.0 / 3.0;
        for (int j = 0; j < i; j++) {
            ld temp = k1 * cur - k2 * t[j];
            t[j] = cur; cur = temp;
            k2 /= 4 * k1 - k2;
            k1 = k2 + 1;
        }
        t.pb(cur);
        if (abs(last - cur) < eps) break;
    }
    return t.back();
}