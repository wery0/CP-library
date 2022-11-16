static int f(String t) {
    t += '+';
    int d = t.length(), o = 0, i = 0, k = 0, v = 1, g = 0;
    char[] c = t.toCharArray();
    for (int q = 0; q < d; q++) {
        if (c[q] == '(') {
            i = q;
            for (g = 1; g != 0; ) {
                q++;
                if (c[q] == ')') g--;
                if (c[q] == '(') g++;
            }
            k = f(t.substring(i + 1, q));
            q++;
        }
        if (c[q] > 47 & c[q] < 58) {
            k = k * 10 + c[q] - 48;
        }
        else {
            o = v == 1 ? o + k : v == 2 ? o - k : o * k;
            v = c[q] == '+' ? 1 : c[q] == '-' ? 2 : 3;
            k = 0;
        }
    }
    return o;
}