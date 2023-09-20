//Dynamic bitset implementation
//Stress test: https://pastebin.com/sxyyGuZh
template<typename T = uint64_t>
class custom_bitset {
    static_assert(is_unsigned_v<T> || is_same_v<T, __uint128_t>);

    static constexpr size_t BIT = 8 * sizeof(T);
    static constexpr size_t BITM = BIT - 1;
    static constexpr size_t LOG = __lg(BIT);
    static constexpr T ONE = 1;
    static constexpr T ALL = -1;
    static inline bool initialized;
    static inline T o[BIT + 1][BIT];

    size_t N;
    vector<T> m;

    void init() {
        initialized = true;
        for (size_t l = 0; l < BIT; ++l) {
            o[l][l] = ONE << l;
            for (size_t r = 0; r < l; ++r) {
                o[l][r] = 0;
            }
            for (size_t r = l + 1; r < BIT; ++r) {
                o[l][r] = o[l][r - 1] | (ONE << r);
            }
        }
        for (size_t r = 0; r < BIT; ++r) {
            o[BIT][r] = 0;
        }
    }

    int popcount64(uint64_t x) {
        x -= (x >> 1) & 0x5555555555555555;
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
        return ((x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f) * 0x0101010101010101 >> 56;
    }

    static int __builtin_popcount128(__uint128_t x) {return __builtin_popcountll(x) + __builtin_popcountll(x >> 64);}
    static int __lg128(__uint128_t x) {return (x >> 64) ? 64 + __lg((uint64_t) (x >> 64)) : __lg((uint64_t) x);}
    static int __builtin_ctz128(__uint128_t x) {return (x << 64) ? __builtin_ctzll((uint64_t) x) : 64 + __builtin_ctzll((uint64_t) (x >> 64));}
    static int __builtin_clz128(__uint128_t x) {return (x >> 64) ? __builtin_clzll((uint64_t) (x >> 64)) : 64 + __builtin_clzll((uint64_t) x);}
    static int __builtin_parity128(__uint128_t x) {return __builtin_parityll((uint64_t) x) ^ __builtin_parityll((uint64_t) (x >> 64));}

    template<typename U>
    static int popcount(U x) {
        if constexpr (sizeof(x) <= 4) {
            return __builtin_popcount(x);
        } else if constexpr (sizeof(x) <= 8) {
            return __builtin_popcountll(x);
        } else {
            return __builtin_popcount128(x);
        }
    }

    template<typename U>
    static int ctz(U x) {
        if constexpr (sizeof(x) <= 4) {
            return __builtin_ctz(x);
        } else if constexpr (sizeof(x) <= 8) {
            return __builtin_ctzll(x);
        } else {
            return __builtin_ctz128(x);
        }
    }

    template<typename U>
    static int lg(U x) {
        if constexpr (sizeof(x) <= 8) {
            return __lg(x);
        } else {
            return __lg128(x);
        }
    }

    void self_shift_left(size_t k) {
        if (k >= N) {
            reset();
            return;
        }
        if ((k & BITM) == 0) {
            fill(m.end() - k / BIT, m.end(), 0);
            rotate(m.begin(), m.end() - k / BIT, m.end());
            set_extra_bits_to_zero();
            return;
        }
        size_t suf = k & BITM, pref = BIT - suf;
        size_t i = m.size() - 1, j = i - k / BIT;
        while (j) {
            m[i--] = (m[j] << suf) | (m[j - 1] >> pref);
            --j;
        }
        m[i--] = m[j] << suf;
        fill(m.begin(), m.begin() + i + 1, 0);
        set_extra_bits_to_zero();
    }

    void self_shift_right(size_t k) {
        if (k >= N) {
            reset();
            return;
        }
        if ((k & BITM) == 0) {
            fill(m.begin(), m.begin() + k / BIT, 0);
            rotate(m.begin(), m.begin() + k / BIT, m.end());
            return;
        }
        size_t suf = k & BITM, pref = BIT - suf;
        size_t i = 0, j = k / BIT;
        while (j + 1 < m.size()) {
            m[i++] = (m[j] >> suf) | (m[j + 1] << pref);
            ++j;
        }
        m[i++] = m[j] >> suf;
        fill(m.begin() + i, m.end(), 0);
    }

    enum OP_TYPE {OR, AND, XOR};
    template<OP_TYPE type>
    void shift_left_op(size_t k) {
        if (k >= N) {
            if constexpr (type == AND) reset();
            return;
        }
        constexpr auto f = [](T & x, T y) {
            if constexpr (type == OR) x |= y;
            else if constexpr (type == AND) x &= y;
            else if constexpr (type == XOR) x ^= y;
            else assert(0 && "Unknown type!");
        };
        if ((k & BITM) == 0) {
            for (ssize_t nk = k >> LOG, i = m.size() - 1; i >= nk; --i) {
                f(m[i], m[i - nk]);
            }
            if constexpr (type == AND) {
                fill(m.begin(), m.begin() + (k >> LOG), 0);
            }
            set_extra_bits_to_zero();
            return;
        }
        size_t suf = k & BITM, pref = BIT - suf;
        size_t i = m.size() - 1, j = i - k / BIT;
        while (j) {
            f(m[i--], (m[j] << suf) | (m[j - 1] >> pref));
            --j;
        }
        f(m[i--], m[j] << suf);
        if constexpr (type == AND) {
            fill(m.begin(), m.begin() + (k >> LOG), 0);
        }
        set_extra_bits_to_zero();
    }

    template<OP_TYPE type>
    void shift_right_op(size_t k) {
        if (k >= N) {
            if constexpr (type == AND) reset();
            return;
        }
        constexpr auto f = [](T & x, T y) {
            if constexpr (type == OR) x |= y;
            else if constexpr (type == AND) x &= y;
            else if constexpr (type == XOR) x ^= y;
            else assert(0 && "Unknown type!");
        };
        if ((k & BITM) == 0) {
            for (size_t nk = k >> LOG, i = 0; i + nk < m.size(); ++i) {
                f(m[i], m[i + nk]);
            }
            if constexpr (type == AND) {
                fill(m.rbegin(), m.rbegin() + (k >> LOG), 0);
            }
            return;
        }
        size_t suf = k & BITM, pref = BIT - suf;
        size_t i = 0, j = k / BIT;
        while (j + 1 < m.size()) {
            f(m[i++], (m[j] >> suf) | (m[j + 1] << pref));
            ++j;
        }
        f(m[i++], m[j] >> suf);
        if constexpr (type == AND) {
            fill(m.rbegin(), m.rbegin() + (k >> LOG), 0);
        }
    }

    void set_extra_bits_to_zero() {
        if (N & BITM) m.back() &= ~o[N & BITM][BIT - 1];
    }

    void set_extra_bits_to_one() {
        if (N & BITM) m.back() |= o[N & BITM][BIT - 1];
    }

public:
    custom_bitset(size_t N = 0) : N(N), m((N + BITM) / BIT) {
        if (!initialized) init();
    }

    custom_bitset(const string& s) : N(s.size()), m((N + BITM) / BIT) {
        if (!initialized) init();
        for (size_t i = 0; i < s.size(); ++i) {
            assign(i, s[i] == '1');
        }
    }

    size_t size() const { return N; }

    void resize(size_t new_N) {
        m.resize((new_N + BITM) / BIT);
        N = new_N;
        set_extra_bits_to_zero();
    }

    void clear() {
        N = 0;
        m.clear();
    }

    size_t count() {
        size_t res = 0;
        for (T mask : m) res += popcount(mask);
        return res;
    }

    size_t count(size_t l, size_t r) const {
        if (l / BIT == r / BIT) {
            return popcount(m[l >> LOG] & o[l & BITM][r & BITM]);
        }
        size_t res = popcount(m[l >> LOG] & o[l & BITM][BITM]) + popcount(m[r >> LOG] & o[0][r & BITM]);
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) res += popcount(m[i]);
        return res;
    }

    bool any_1() {
        for (T mask : m) if (mask) return true;
        return false;
    }

    bool any_1(size_t l, size_t r) const {
        if (l / BIT == r / BIT) {
            return m[l >> LOG] & o[l & BITM][r & BITM];
        }
        if (m[l >> LOG] & o[l & BITM][BITM]) return true;
        if (m[r >> LOG] & o[0][r & BITM]) return true;
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) {
            if (m[i]) return true;
        }
        return false;
    }

    bool all_1() {
        set_extra_bits_to_one();
        for (T mask : m) {
            if (mask != ALL) {
                set_extra_bits_to_zero();
                return false;
            }
        }
        set_extra_bits_to_zero();
        return true;
    }

    bool all_1(size_t l, size_t r) const {
        if (l / BIT == r / BIT) {
            return (m[l >> LOG] & o[l & BITM][r & BITM]) == o[l & BITM][r & BITM];
        }
        if ((m[l >> LOG] & o[l & BITM][BITM]) != o[l & BITM][BITM]) return false;
        if ((m[r >> LOG] & o[0][r & BITM]) != o[0][r & BITM]) return false;
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) {
            if (m[i] != ALL) return false;
        }
        return true;
    }

    bool all_0() {
        return !any_1();
    }

    bool all_0(size_t l, size_t r) const {
        return !any_1(l, r);
    }

    bool any_0() {
        return !all_1();
    }

    bool any_0(size_t l, size_t r) const {
        return !all_1(l, r);
    }

    bool get(size_t i) const {
        return m[i >> LOG] >> (i & BITM) & 1;
    }

    void assign(size_t i, bool val) {
        val ? set(i) : reset(i);
    }

    void set(size_t i) {
        m[i >> LOG] |= ONE << (i & BITM);
    }

    void set() {
        fill(m.begin(), m.end(), ALL);
        set_extra_bits_to_zero();
    }

    void set(size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] |= o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] |= o[l & BITM][BITM];
        m[r >> LOG] |= o[0][r & BITM];
        fill(m.begin() + l / BIT + 1, m.begin() + r / BIT, ALL);
    }

    //*this[i] = rhs[i], l <= i <= r
    void copy_seg(const custom_bitset<T>& rhs, size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] = (m[l >> LOG] & ~o[l & BITM][r & BITM]) | rhs.m[l >> LOG] & o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] = (m[l >> LOG] & ~o[l & BITM][BITM]) | rhs.m[l >> LOG] & o[l & BITM][BITM];
        m[r >> LOG] = (m[r >> LOG] & ~o[0][r & BITM]) | rhs.m[r >> LOG] & o[0][r & BITM];
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) m[i] = rhs.m[i];
    }

    //*this[i] |= rhs[i], l <= i <= r
    void or_seg(const custom_bitset<T>& rhs, size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] |= rhs.m[l >> LOG] & o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] |= rhs.m[l >> LOG] & o[l & BITM][BITM];
        m[r >> LOG] |= rhs.m[r >> LOG] & o[0][r & BITM];
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) m[i] |= rhs.m[i];
    }

    //*this[i] &= rhs[i], l <= i <= r
    void and_seg(const custom_bitset<T>& rhs, size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] &= rhs.m[l >> LOG] | ~o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] &= rhs.m[l >> LOG] | ~o[l & BITM][BITM];
        m[r >> LOG] &= rhs.m[r >> LOG] | ~o[0][r & BITM];
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) m[i] &= rhs.m[i];
    }

    //*this[i] ^= rhs[i], l <= i <= r
    void xor_seg(const custom_bitset<T>& rhs, size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] ^= rhs.m[l >> LOG] & o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] ^= rhs.m[l >> LOG] & o[l & BITM][BITM];
        m[r >> LOG] ^= rhs.m[r >> LOG] & o[0][r & BITM];
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) m[i] ^= rhs.m[i];
    }

    void reset(size_t i) {
        m[i >> LOG] &= ALL ^ (ONE << (i & BITM));
    }

    void reset() {
        fill(m.begin(), m.end(), 0);
    }

    void reset(size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] &= ~o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] &= ~o[l & BITM][BITM];
        m[r >> LOG] &= ~o[0][r & BITM];
        fill(m.begin() + l / BIT + 1, m.begin() + r / BIT, 0);
    }

    void flip(size_t i) {
        assert(i < N);
        m[i >> LOG] ^= ONE << (i & BITM);
    }

    void flip() {
        for (T& mask : m) mask = ~mask;
        set_extra_bits_to_zero();
    }

    void flip(size_t l, size_t r) {
        if (l / BIT == r / BIT) {
            m[l >> LOG] ^= o[l & BITM][r & BITM];
            return;
        }
        m[l >> LOG] ^= o[l & BITM][BITM];
        m[r >> LOG] ^= o[0][r & BITM];
        for (size_t i = l / BIT + 1; i < r / BIT; ++i) m[i] = ~m[i];
    }

    custom_bitset& operator|=(const custom_bitset& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < m.size(); ++i) m[i] |= rhs.m[i];
        return *this;
    }

    friend custom_bitset operator|(const custom_bitset& lhs, const custom_bitset& rhs) {
        custom_bitset res = lhs;
        res |= rhs;
        return res;
    }

    custom_bitset& operator&=(const custom_bitset& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < m.size(); ++i) m[i] &= rhs.m[i];
        return *this;
    }

    friend custom_bitset operator&(const custom_bitset& lhs, const custom_bitset& rhs) {
        custom_bitset res = lhs;
        res &= rhs;
        return res;
    }

    custom_bitset& operator^=(const custom_bitset& rhs) {
        assert(size() == rhs.size());
        for (size_t i = 0; i < m.size(); ++i) m[i] ^= rhs.m[i];
        return *this;
    }

    friend custom_bitset operator^(const custom_bitset& lhs, const custom_bitset& rhs) {
        custom_bitset res = lhs;
        res ^= rhs;
        return res;
    }

    custom_bitset operator~() const {
        custom_bitset res = *this;
        res.flip();
        return res;
    }

    bool operator==(const custom_bitset& rhs) const {
        return N == rhs.N && m == rhs.m;
    }

    size_t find_first_0() const {
        for (size_t i = 0; i < N; ++i) {
            if (m[i] != ALL) return min(N, (i << LOG) | ctz(~m[i]));
        }
        return N;
    }

    size_t find_last_0() {
        if ((N & BITM) && m.back() != o[0][(N & BITM) - 1]) {
            return (N & ~BITM) | lg(~m.back() & o[0][(N & BITM) - 1]);
        }
        for (ssize_t i = (ssize_t) m.size() - ((N & BITM) ? 2 : 1); i >= 0; --i) {
            if (m[i] != ALL) return (i << LOG) | lg(~m[i]);
        }
        return N;
    }

    size_t find_first_1() const {
        for (size_t i = 0; i < N; ++i) {
            if (m[i]) return min(N, (i << LOG) | ctz(m[i]));
        }
        return N;
    }

    size_t find_last_1() const {
        if ((N & BITM) && m.back()) {
            return (N & ~o[0][LOG - 1]) | lg(m.back());
        }
        for (ssize_t i = (ssize_t) m.size() - ((N & BITM) ? 2 : 1); i >= 0; --i) {
            if (m[i]) return (i << LOG) | lg(m[i]);
        }
        return N;
    }

    size_t find_next_1(size_t i) const {
        size_t id = i >> LOG;
        if (size_t rem = i & BITM; m[id] && lg(m[id]) > rem) {
            return min(N, i + ctz(m[id] >> (rem + 1)) + 1);
        }
        while (++id < m.size()) {
            if (m[id]) return (id << LOG) | ctz(m[id]);
        }
        return N;
    }

    size_t find_prev_1(size_t i) const {
        size_t id = i >> LOG;
        if (size_t rem = i & BITM; m[id] && ctz(m[id]) < rem) {
            return (id << LOG) | lg(m[id] & o[0][rem - 1]);
        }
        while (id--) {
            if (m[id]) return (id << LOG) | lg(m[id]);
        }
        return N;
    }

    size_t find_next_0(size_t i) const {
        size_t id = i >> LOG;
        if (size_t rem = i & BITM; ~m[id] && lg(~m[id]) > rem) {
            return min(N, i + ctz(~m[id] >> (rem + 1)) + 1);
        }
        while (++id < m.size()) {
            if (~m[id]) return min(N, (id << LOG) | ctz(~m[id]));
        }
        return N;
    }

    size_t find_prev_0(size_t i) const {
        size_t id = i >> LOG;
        if (size_t rem = i & BITM; ~m[id] && ctz(~m[id]) < rem) {
            return (id << LOG) | lg(~m[id] & o[0][rem - 1]);
        }
        while (id--) {
            if (~m[id]) return (id << LOG) | lg(~m[id]);
        }
        return N;
    }

    custom_bitset operator<<(size_t k) const {
        custom_bitset res = *this;
        res <<= k;
        return res;
    }

    custom_bitset operator>>(size_t k) const {
        custom_bitset res = *this;
        res >>= k;
        return res;
    }

    //<=> *this |= *this << k
    void or_shift_left(size_t k) {
        shift_left_op<OR>(k);
    }

    //<=> *this &= *this << k
    void and_shift_left(size_t k) {
        shift_left_op<AND>(k);
    }

    //<=> *this ^= *this << k
    void xor_shift_left(size_t k) {
        shift_left_op<XOR>(k);
    }

    //<=> *this |= *this >> k
    void or_shift_right(size_t k) {
        shift_right_op<OR>(k);
    }

    //<=> *this &= *this >> k
    void and_shift_right(size_t k) {
        shift_right_op<AND>(k);
    }

    //<=> *this ^= *this >> k
    void xor_shift_right(size_t k) {
        shift_right_op<XOR>(k);
    }

    void operator<<=(size_t k) {
        self_shift_left(k);
    }

    void operator>>=(size_t k) {
        self_shift_right(k);
    }

    friend ostream& operator<<(ostream& os, const custom_bitset& rhs) {
        for (size_t i = 0; i < rhs.N; ++i) os << rhs.get(i);
        return os;
    }
};
