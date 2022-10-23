ll Bgrid_dst(ll b, ll x1, ll y1, ll x2, ll y2) {
    //assert(x1 % b == 0 || y1 % b == 0); assert(x2 % b == 0 || y2 % b == 0);
    auto dst = [&](ll x1, ll y1, ll x2, ll y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    };
    ll d = dst(x1, y1, x2, y2);
    if ((x1 == x2 && x1 % b == 0) || (y1 == y2 && y1 % b == 0)) return d;
    if ((x1 % b == 0 && y1 % b == 0) || (x2 % b == 0 && y2 % b == 0)) return d;
    if ((x1 % b == 0 && y2 % b == 0) || (x2 % b == 0 && y1 % b == 0)) return d;
    if (x1 % b == 0) swap(x1, y1), swap(x2, y2);
    if (x1 / b != x2 / b) return d;
    ll lx1 = x1 - x1 % b, rx1 = lx1 + b;
    ll d1 = abs(x1 - lx1) + dst(lx1, y1, x2, y2);
    ll d2 = abs(x1 - rx1) + dst(rx1, y1, x2, y2);
    return min(d1, d2);
}
//points (x1, y1), (x2, y2) should lie on line x = bZ or y = bZ.