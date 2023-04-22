constexpr uint64_t lowest_bitsll[] = {0ull, 1ull, 3ull, 7ull, 15ull, 31ull, 63ull, 127ull, 255ull, 511ull, 1023ull, 2047ull, 4095ull, 8191ull, 16383ull, 32767ull, 65535ull, 131071ull, 262143ull, 524287ull, 1048575ull, 2097151ull, 4194303ull, 8388607ull, 16777215ull, 33554431ull, 67108863ull, 134217727ull, 268435455ull, 536870911ull, 1073741823ull, 2147483647ull, 4294967295ull, 8589934591ull, 17179869183ull, 34359738367ull, 68719476735ull, 137438953471ull, 274877906943ull, 549755813887ull, 1099511627775ull, 2199023255551ull, 4398046511103ull, 8796093022207ull, 17592186044415ull, 35184372088831ull, 70368744177663ull, 140737488355327ull, 281474976710655ull, 562949953421311ull, 1125899906842623ull, 2251799813685247ull, 4503599627370495ull, 9007199254740991ull, 18014398509481983ull, 36028797018963967ull, 72057594037927935ull, 144115188075855871ull, 288230376151711743ull, 576460752303423487ull, 1152921504606846975ull, 2305843009213693951ull, 4611686018427387903ull, 9223372036854775807ull, 18446744073709551615ull};
const uint32_t NO = UINT32_MAX;   //This value will be returned in lower_bound functions, if no answer exists. Change, if need.
template<uint32_t MAXN>           //Can correctly work with numbers in range [0; MAXN]
class intset_fast {
    static const uint32_t SZ = (MAXN + 64) / 64;
    static const uint32_t U = SZ & (SZ - 1) ? 2u << __lg(SZ) : SZ;
    bool kek[U * 2] = {0};
    uint64_t m[SZ] = {0};

    inline uint32_t left_go(uint32_t x, const uint32_t c) const {
        const uint64_t rem = x & 63;
        uint64_t num = m[x >> 6] & lowest_bitsll[rem + c];
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
        uint64_t num = m[x >> 6] & ~lowest_bitsll[rem + c];
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
//Supports all std::set operations in O(1) on random queries / dense arrays, O(log_2(N/64)) in worst case (sparce array).
//Count operation works in O(1) always.
//Memory usage: at most 3 * MAXN bits.