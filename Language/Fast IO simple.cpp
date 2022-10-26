inline char getChar() {
    static char buf[1 << 16];
    static int ps = 0, sz = 0;
    if (ps == sz) {
        ps = 0;
        sz = fread(buf, sizeof(buf[0]), sizeof(buf) / sizeof(buf[0]), stdin);
        if (sz == 0) return EOF;
    }
    return buf[ps++];
}

inline void putChar(int c) {
    static char buf[1 << 16];
    static int pos = 0;
    if (c == EOF || pos == 1 << 16) {
        fwrite(buf, sizeof(buf[0]), pos, stdout);
        pos = 0;
    }
    if (c == EOF) return;
    buf[pos++] = c;
}

inline int ni() {
    char c = getChar();
    for (; c == '\n' || c == ' ';) c = getChar();
    int o = 0, sign = 1;
    if (c == '-') sign = -1, c = getChar();
    for (; '0' <= c && c <= '9'; c = getChar()) o = o * 10 + c - '0';
    return o * sign;
}

inline ll nl() {
    char c = getChar();
    for (; c == '\n' || c == ' ';) c = getChar();
    ll o = 0, sign = 1;
    if (c == '-') sign = -1, c = getChar();
    for (; '0' <= c && c <= '9'; c = getChar()) o = o * 10 + c - '0';
    return o * sign;
}

inline void printInt(int val) {
    static char buf[12];
    int pos = 0, sign = val < 0 ? -1 : 1;
    val = abs(val);
    buf[pos++] = '\n';
    if (val == 0) buf[pos++] = 48;
    for (; val; val /= 10) buf[pos++] = 48 + val % 10;
    if (sign == -1) buf[pos++] = '-';
    for (; --pos >= 0;) putChar(buf[pos]);
}

inline void printLong(ll val) {
    static char buf[21];
    int pos = 0, sign = val < 0 ? -1 : 1;
    val = abs(val);
    buf[pos++] = '\n';
    if (val == 0) buf[pos++] = 48;
    for (; val; val /= 10) buf[pos++] = 48 + val % 10;
    if (sign == -1) buf[pos++] = '-';
    for (; --pos >= 0;) putChar(buf[pos]);
}