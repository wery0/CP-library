//Calculates number of primes in range [1, N]
template<typename T>
T count_primes(const T N) {
    assert(N <= 1e18);
    if (N < 2) return 0;
    const int v = sqrtl(N);
    int s = (v + 1) / 2;
    vector<int> smalls(s);
    vector<int> roughs(s);
    vector<T> larges(s);
    vector<char> skip(v + 1);
    iota(smalls.begin(), smalls.end(), 0);
    for (int i = 0; i < s; i++) roughs[i] = 2 * i + 1;
    for (int i = 0; i < s; i++) larges[i] = (N / (2 * i + 1) - 1) / 2;
    const auto divide = [](T n, T d) -> int {return n>>53 ? (long double)(n) / d : double(n) / d;};
    const auto half = [](int n) -> int {return (n - 1) >> 1;};
    int pc = 0;
    for (int p = 3; p <= v; p += 2) if (!skip[p]) {
        T q = p * p;
        if (q * q > N) break;
        skip[p] = true;
        for (int i = q; i <= v; i += 2 * p) skip[i] = true;
        int ns = 0;
        for (int k = 0; k < s; k++) {
            int i = roughs[k];
            if (skip[i]) continue;
            T d = T(i) * p;
            larges[ns] = larges[k] - (d <= v ? larges[smalls[d >> 1] - pc] : smalls[half(divide(N, d))]) + pc;
            roughs[ns++] = i;
        }
        s = ns;
        for (int i = half(v), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
            int c = smalls[j >> 1] - pc;
            for (int e = (j * p) >> 1; i >= e; i--) smalls[i] -= c;
        }
        pc++;
    }
    larges[0] += T(s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int k = 1; k < s; k++) larges[0] -= larges[k];
    for (int l = 1; l < s; l++) {
        T q = roughs[l];
        T M = N / q;
        int e = smalls[half(M / q)] - pc;
        if (e < l + 1) break;
        T t = 0;
        for (int k = l + 1; k <= e; k++) t += smalls[half(divide(M, roughs[k]))];
        larges[0] += t - T(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}
