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

ll extended_inv(ll a, ll mod) {
    ll x, y, g = gcd(a, mod, x, y);
    if (g != 1) return -1;
    if (x < 0) x += (-x + mod - 1) / mod * mod;
    x %= mod;
    return x;
}

const ll mod = 1e9 + 7;
const ll G = 3e5 + 5;
ll fc[G], ifc[G];

ll powmod(ll x, ll k) {
    if (k < 0) return 0;
    ll o = 1;
    for (; k; k >>= 1) {
        if (k & 1) o = o * x % mod;
        x = x * x % mod;
    }
    return o;
}

ll inv(ll x) {return powmod(x, mod - 2);}

ll C(ll n, ll k) {return n < 0 || k < 0 || k > n ? 0 : fc[n] * ifc[k] % mod * ifc[n - k] % mod;}

int main() {
    fast;
    fc[0] = 1;
    for (int q = 1; q < G; q++) fc[q] = fc[q - 1] * q % mod;
    ifc[G - 1] = inv(fc[G - 1]);
    for (int q = G - 2; q >= 0; q--) ifc[q] = ifc[q + 1] * (q + 1) % mod;
}