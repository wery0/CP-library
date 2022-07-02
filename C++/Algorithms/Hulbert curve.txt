ll rec(int x, int y, int n, int rot) {
    if (n == 0) return 0;
    int s = 1 << n - 1, p = x <= s ? y <= s ? 1 : 2 : y <= s ? 4 : 3;
    if (rot == 1) p = p % 2 == 0 ? (p + 1) % 4 + 1 : p;
    if (rot == 2) p = (p + 1) % 4 + 1;
    if (rot == 3) p = p % 2 == 1 ? (p + 1) % 4 + 1 : p;
    if (p == 1 || p == 4) {
        if ((p + rot) % 2 == 1) rot = (rot + 1) % 4;
        else rot = (rot + 3) % 4;
    }
    return 1LL * s * s * (p - 1) + rec(x - (x > s ? s : 0), y - (y > s ? s : 0), n - 1, rot);
}

rec(l, r, 20, 0);