template<const int N>
struct Bitset {
    const static int sz = (N + 63) / 64, extra = 64 - sz * 64 + N;
    ull a[sz];
    static inline int cnt = 0;
    const ull allBits = -1;
    static inline ull o[64][64], z[64][64];

    void init() {
        for (int i = 0; i < 64; i++) {
            o[i][i] = 1ull << i;
            z[i][i] = allBits ^ o[i][i];
            for (int j = i + 1; j < 64; j++) {
                o[i][j] = o[i][j - 1] | (1ull << j);
                z[i][j] = allBits ^ o[i][j];
            }
        }
    }

    Bitset() {
        if (cnt == 0) init();
        ++cnt;
        memset(a, 0, sizeof a);
    }

    inline bool operator[](const int i) {
        return a[i >> 6] >> (i & 63) & 1;
    }

    inline void set(int i) {
        a[i / 64] |= (1ull << (i & 63));
    }

    inline void set() {
        for (int i = 0; i < sz; i++) a[i] = allBits;
    }

    inline void reset(int i) {
        a[i / 64] &= (allBits ^ (1ull << (i & 63)));
    }

    inline void reset() {
        memset(a, 0, sizeof a);
    }

    inline void flip(int i) {
        a[i / 64] ^= (1ull << (i & 63));
    }

    inline int count() {
        int ans = 0;
        for (int i = 0; i < sz; i++) ans += __builtin_popcountll(a[i]);
        return ans;
    }

    inline void set(int l, int r) {
        if (l / 64 == r / 64) {
            a[l / 64] |= o[l & 63][r & 63];
            return;
        }
        a[l / 64] |= o[l & 63][63];
        a[r / 64] |= o[0][r & 63];
        for (int i = l / 64 + 1; i < r / 64; i++) a[i] = allBits;
    }

    inline void reset(int l, int r) {
        if (l / 64 == r / 64) {
            a[l / 64] &= z[l & 63][r & 63];
            return;
        }
        a[l / 64] &= z[l & 63][63];
        a[r / 64] &= z[0][r & 63];
        for (int i = l / 64 + 1; i < r / 64; i++) a[i] = 0;
    }

    inline void flip(int l, int r) {
        if (l / 64 == r / 64) {
            a[l / 64] ^= o[l & 63][r & 63];
            return;
        }
        a[l / 64] ^= o[l & 63][63];
        a[r / 64] ^= o[0][r & 63];
        for (int i = l / 64 + 1; i < r / 64; i++) a[i] = ~a[i];
    }

    inline Bitset operator&(const Bitset &x) const {
        Bitset ans;
        for (int i = 0; i < sz; i++) ans.a[i] = a[i] & x.a[i];
        return ans;
    }

    inline Bitset operator|(const Bitset &x) const {
        Bitset ans;
        for (int i = 0; i < sz; i++) ans.a[i] = a[i] | x.a[i];
        return ans;
    }

    inline Bitset operator^(const Bitset &x) const {
        Bitset ans;
        for (int i = 0; i < sz; i++) ans.a[i] = a[i] ^ x.a[i];
        return ans;
    }

    inline Bitset operator<<(const int k) const {
        Bitset b;
        int s1 = k & 63, s2 = 64 - s1;
        for (int i = sz - 1, j = i - (k + 63) / 64; j > -2; i--, j--) {
            b.a[i] = a[j + 1] << s1;
            if (j > -1) b.a[i] |= a[j] >> s2;
        }
        if (extra != 64) b.a[sz - 1] ^= (b.a[sz - 1] >> extra) << extra;
        return b;
    }

    inline Bitset operator>>(const int k) const {
        Bitset b;
        int s1 = k & 63, s2 = 64 - s1;
        for (int i = 0, j = (k + 63) / 64; j <= sz; i++, j++) {
            b.a[i] = a[j - 1] >> s1;
            if (j < sz) b.a[i] |= a[j] << s2;
        }
        return b;
    }

    inline bool operator==(const Bitset &x) const {
        for (int i = 0; i < sz; i++) if (a[i] != x.a[i]) return false;
        return true;
    }

    inline bool operator>(const Bitset &x) const {
        for (int i = sz - 1; i >= 0; i--) if (a[i] != x.a[i]) return a[i] > x.a[i];
        return false;
    }

    inline bool operator<(const Bitset &x) const {
        for (int i = sz - 1; i >= 0; i--) if (a[i] != x.a[i]) return a[i] < x.a[i];
        return false;
    }

    friend Bitset operator~(const Bitset &x) {
        Bitset ans;
        for (int i = 0; i < sz; i++) ans.a[i] = ~x.a[i];
        return ans;
    }

    inline int _Find_first() const {
        for (int i = 0, ans = 0; i < sz; i++, ans += 64) {
            if (a[i]) return ans + __builtin_ctzll(a[i]);
        }
        return N;
    }

    inline int _Find_next(int k) const {
        const int id = k >> 6, rem = k & 63;
        if (a[id] && __lg(a[id]) > rem) {
            return id * 64 + rem + (__builtin_ctzll(a[id] >> (rem + 1))) + 1;
        }
        for (int i = id + 1, ans = (id + 1) * 64; i < sz; i++, ans += 64) {
            if (a[i]) return ans + __builtin_ctzll(a[i]);
        }
        return N;
    }
};