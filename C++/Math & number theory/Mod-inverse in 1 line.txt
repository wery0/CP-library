//to prevent overflow, x^2 and m^2 should fit in long long.
ll inv(ll x, ll m) {
    return x == 1 ? x : (1 - inv(m % x, x) * m) / x + m;
}