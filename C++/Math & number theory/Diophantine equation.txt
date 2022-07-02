namespace diophantine_equation {
    ll gcd(ll a, ll b, ll &x, ll &y) {
        if (!a) {
            x = 0, y = 1;
            return b;
        }
        ll g = gcd(b % a, a, x, y);
        ll nx = y - x * (b / a), ny = x;
        x = nx, y = ny;
        return g;
    }

    //returns solution with minimal positive x.
    pll solve(ll A, ll B, ll C) {
        ll x, y, g = gcd(abs(A), abs(B), x, y);
        if (A < 0) x *= -1;
        if (B < 0) y *= -1;
        //assert(C % g == 0);
        if (C % g) return { -1, -1};
        x *= C / g, y *= C / g;
        ll dx = B / g, dy = -A / g;
        ll c = x / dx;
        x -= c * dx;
        y -= c * dy;
        if (x <= 0) {
            if (dx > 0) x += dx, y += dy;
            else x -= dx, y -= dy;
        }
        assert(x > 0);
        return {x, y};
    }
};