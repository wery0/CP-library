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
    while (c <= 32) c = getChar();
    int res = 0, sign = 1;
    if (c == '-') sign = -1, c = getChar();
    for (; isdigit(c); c = getChar()) res = res * 10 + c - 48;
    return res * sign;
}

inline int64_t nl() {
    char c = getChar();
    while (c <= 32) c = getChar();
    int64_t o = 0, sign = 1;
    if (c == '-') sign = -1, c = getChar();
    for (; isdigit(c); c = getChar()) o = o * 10 + c - 48;
    return o * sign;
}

inline string ns() {
	char c = getChar();
    while (c <= 32) c = getChar();
    string res;
    for (; c > 32; c = getChar()) res += c;
    return res;
}

template<typename T>
inline void print_signed(T val) {
    static_assert(is_integral_v<T> && is_signed_v<T>);
    static char buf[24];
    int pos = 0, sign = val < 0 ? -1 : 1;
    val = abs(val);
    buf[pos++] = '\n';
    if (val == 0) buf[pos++] = 48;
    for (; val; val /= 10) buf[pos++] = 48 + val % 10;
    if (sign == -1) buf[pos++] = '-';
    while (--pos >= 0) putChar(buf[pos]);
}

inline void print_string(const string& s) {
	for (char c : s) putChar(c);
}
