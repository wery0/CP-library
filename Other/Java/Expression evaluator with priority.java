double f(String t) {
    t += '+';
    double d = t.length(), o = 0, i = 0, k = 0, v = 1, p = 1, j = 0, g = 0, cnt = 0, dist = 0;
    char[] c = t.toCharArray();
    boolean pnt = false;
    for (int q = 0; q < d; q++) {
        if (c[q] == '(') {
            pnt = false;
            i = q;
            for (g = 1; g != 0; ) {
                q++;
                if (c[q] == ')') g--;
                if (c[q] == '(') g++;
            }
            k = f(t.substring((int)i + 1, q));
            q++;
        }
        if (c[q] == 's' || c[q] == 'c') {
            pnt = false;
            String ss = "(";
            boolean sin = c[q] == 's';
            q += 4;
            for (g = 1; g != 0; q++) {
                if (c[q] == '(') g++;
                if (c[q] == ')') g--;
                ss += c[q];
            }
            q--;
            k = sin ? Math.sin(f(ss)) : Math.cos(f(ss));
        }
        else if ((c[q] > 47 && c[q] < 58) || c[q] == '.') {
            if (c[q] == '.') {
                pnt = true;
                dist = 1;
                continue;
            }
            if (pnt) {
                k += (c[q] - 48) * Math.pow(10, -dist);
                dist++;
            }
            else k = k * 10 + c[q] - 48;
        }
        else {
            pnt = false;
            if (c[q] != '*' && c[q] != '/') {
                o = j > 0 ? (j == 1 ? v == 1 ? o + p*k : o - p*k : v == 1 ? o + p / k : o - p / k) : v == 1 ? o + k : o - k;
                v = c[q] == '+' ? 1 : 2;
                p = 1;
                j = 0;
                cnt = 0;
            }
            else {
                p = j == 1 || cnt == 0 ? p * k : p / k;
                j = c[q] == '*' ? 1 : 2;
                cnt++;
            }
            k = 0;
        }
    }
    return o;
}