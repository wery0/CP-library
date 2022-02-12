const ld EPS = 1e-8;
const int G = 30;
ll a;
ld m[G][G];

void GAUSS() {
    for (int q = 0, w = 0; q < a && w < a; q++, w++) {
        int i;
        for (; w < a; w++) {
            i = q;
            for (; i < a && abs(m[i][w]) < EPS;) i++;
            if (i < a) break;
        }
        if (w == a) break;
        if (i != q) swap(m[q], m[i]);
        const ld k = m[q][w];
        for (int e = 0; e < a + 1; e++) m[q][e] /= k;
        for (int e = 0; e < a; e++) {
            if (e == q) continue;
            const ld u = m[e][w] / m[q][w];
            for (int r = 0; r <= a; r++) {
                m[e][r] -= m[q][r] * u;
            }
        }
    }
    for (int q = 0; q < a; q++) {
        int fl = 1;
        for (int w = 0; w < a; w++) fl &= abs(m[q][w]) < EPS;
        if (fl && abs(m[q][a]) > EPS) {
            cout << "impossible";
            return;
        }
    }
    for (int q = 0; q < a; q++) {
        int fl = 1;
        for (int w = 0; w < a; w++) {
            fl &= abs(m[q][w]) < EPS;
        }
        if (fl) {
            cout << "infinity";
            return;
        }
    }
    cout << "single\n";
    for (int q = 0; q < a; q++) {
        cout << setprecision(12) << fixed << m[q][a] << " ";
    }
}