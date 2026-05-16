inline char nc() {
    static char buf[1 << 16];
    static int ps = 0, sz = 0;
    if (ps == sz) {
        ps = 0;
        sz = fread(buf, sizeof(buf[0]), sizeof(buf) / sizeof(buf[0]), stdin);
        if (sz == 0) return EOF;
    }
    return buf[ps++];
}

inline void pc(int c) {
    static char buf[1 << 16];
    static int pos = 0;
    if (c == EOF || pos == 1 << 16) {
        fwrite(buf, sizeof(buf[0]), pos, stdout);
        pos = 0;
    }
    if (c == EOF) return;
    buf[pos++] = c;
}

template<typename T>
inline T ni() {
    static_assert(is_signed_v<T>);
    char c = nc();
    while (c <= 32) c = nc();
    T res = 0, sign = 1;
    if (c == '-') sign = -1, c = nc();
    for (; isdigit(c); c = nc()) res = res * 10 + c - 48;
    return res * sign;
}

template<typename T>
inline T nui() {
    static_assert(is_unsigned_v<T>);
    char c = nc();
    while (c <= 32) c = nc();
    T res = 0;
    for (; isdigit(c); c = nc()) res = res * 10 + c - 48;
    return res;
}

inline int32_t ni32() {return ni<int32_t>();}
inline int64_t ni64() {return ni<int64_t>();}
inline uint32_t nui32() {return nui<uint32_t>();}
inline uint64_t nui64() {return nui<uint64_t>();}

inline string ns() {
	char c = nc();
    while (c <= 32) c = nc();
    string res;
    for (; c > 32; c = nc()) res += c;
    return res;
}

template<typename T>
inline void print_int(T val) {
    static_assert(is_integral_v<T>);
    static char buf[24];
    if constexpr (is_signed_v<T>) {
        int pos = 0, sign = val < 0 ? -1 : 1;
        val = abs(val);
        if (val == 0) buf[pos++] = 48;
        for (; val; val /= 10) buf[pos++] = 48 + val % 10;
        if (sign == -1) buf[pos++] = '-';
        while (--pos >= 0) pc(buf[pos]);
    } else {
        int pos = 0;
        if (val == 0) buf[pos++] = 48;
        for (; val; val /= 10) buf[pos++] = 48 + val % 10;
        while (--pos >= 0) pc(buf[pos]);
    }
}

inline void print_string(const string& s) {
	for (char c : s) pc(c);
}
