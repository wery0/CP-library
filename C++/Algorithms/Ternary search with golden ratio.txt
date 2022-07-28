const ld FI = (sqrt(5) + 1) / 2.0;
ld SUPERternarka(ld L, ld R) {
    ld p1 = L + (R - L) / (FI + 1), p2 = R - (R - L) / (FI + 1);
    ld v1 = FUNC(p1), v2 = FUNC(p2);
    for (; R - L > EPSILON;) {
        if (v1 < v2) {
            R = p2;
            p2 = p1, v2 = v1;
            p1 = L + (R - L) / (FI + 1), v1 = FUNC(p1);
        } else {
            L = p1;
            p1 = p2, v1 = v2;
            p2 = R - (R - L) / (FI + 1), v2 = FUNC(p2);
        }
    }
    return (L + R) / 2;
}

//faster than usual ternary search in 2log_{1.5}(FI) ~ 2.37 times.