/*
  Can correctly work with numbers in range [0; MAXN]
  Supports all std::set operations in O(1) on random queries / dense arrays, O(log_2(MAXN / 64)) in worst case (sparse array).
  Memory usage: at most MAXN * 3 bits.
  For lower_bound() queries will return NO if no answer exists.
*/
template<uint32_t MAXN, uint32_t NO = UINT32_MAX>
class intset_fast {
    static const uint32_t SZ = (MAXN + 64) / 64;
    static const uint32_t U = SZ & (SZ - 1) ? 2u << __lg(SZ) : SZ;
    bool kek[U * 2] = {0};
    uint64_t m[SZ] = {0};

    static constexpr uint64_t lowest_bits(uint32_t i) {return i ? ~uint64_t(0) >> (64 - i) : 0;}

    inline uint32_t left_go(uint32_t x, const uint32_t c) const {
        const uint64_t rem = x & 63;
        uint64_t num = m[x >> 6] & lowest_bits(rem + c);
        if (num) return (x ^ rem) | __lg(num);
        for (x = (x >> 6) + U; x; x >>= 1) {
            x >>= __builtin_ctzll(x);
            if (kek[x ^ 1]) {x ^= 1; break;}
        }
        if (x == 0) return NO;
        while (x < U) {
            x <<= 1;
            x += kek[x | 1];
        }
        x -= U;
        return (x << 6) | __lg(m[x]);
    }

    inline uint32_t right_go(uint32_t x, const uint32_t c) const {
        const uint64_t rem = x & 63;
        uint64_t num = m[x >> 6] & ~lowest_bits(rem + c);
        if (num) return (x ^ rem) | __builtin_ctzll(num);
        x = (x >> 6) + U;
        for (x >>= __builtin_ctzll(~x); x; x >>= 1, x >>= __builtin_ctzll(~x)) {
            if (kek[x ^ 1]) {x ^= 1; break;}
        }
        if (x == 0) return NO;
        while (x < U) {
            x <<= 1;
            x += !kek[x];
        }
        x -= U;
        return (x << 6) | __builtin_ctzll(m[x]);
    }

public:
    intset_fast() {kek[0] = 1;}

    void operator=(intset_fast<MAXN>& rhs) {
        memcpy(kek, rhs.kek, sizeof(rhs.kek));
        memcpy(m, rhs.m, sizeof(rhs.m));
    }

    void insert(uint32_t p) {
        m[p >> 6] |= 1ull << (p & 63);
        for (p = (p >> 6) + U; !kek[p]; p >>= 1) {
            kek[p] = 1;
        }
    }

    void erase(uint32_t p) {
        m[p >> 6] &= ~(1ull << (p & 63));
        p >>= 6;
        if (m[p]) return;
        for (p += U; p; p >>= 1) {
            kek[p] = 0;
            if (kek[p ^ 1]) break;
        }
    }

    bool empty() const {return kek[1];}
    bool count(uint32_t p) const {return m[p >> 6] >> (p & 63) & 1;}
    void clear() {memset(kek + 1, 0, sizeof(kek[0]) * (U * 2 - 1)); memset(m, 0, sizeof(m[0]) * SZ);}

    uint32_t lower_bound(uint32_t x) const {return right_go(x, 0);}
    uint32_t upper_bound(uint32_t x) const {return right_go(x, 1);}
    uint32_t inverse_lower_bound(uint32_t x) const {return left_go(x, 1);}
    uint32_t inverse_upper_bound(uint32_t x) const {return left_go(x, 0);}
};
