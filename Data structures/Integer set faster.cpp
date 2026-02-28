/*
  Can correctly work with numbers in range [0; MAXN]
  Supports std::set operations in O(1) on random queries / dense arrays, O(log_64(N)) in worst case (sparse array).
  Memory usage: at most MAXN * 2.02 bits.
  For lower_bound() queries will return NO if no answer exists.
*/
template<uint32_t MAXN, uint32_t NO = UINT32_MAX>
class intset_faster {
    static const uint32_t PREF = (MAXN <= 64 ? 0 :
                                  MAXN <= 4096 ? 1 :
                                  MAXN <= 262144 ? 1 + 64 :
                                  MAXN <= 16777216 ? 1 + 64 + 4096 :
                                  MAXN <= 1073741824 ? 1 + 64 + 4096 + 262144 :
                                  1 + 64 + 4096 + 262144 + 16777216) + 1;
    static const uint32_t SZ = PREF + (MAXN + 63) / 64 + 1;
    uint64_t m[SZ] = {0};

    static constexpr uint64_t lowest_bits(uint32_t i) {return i ? ~uint64_t(0) >> (64 - i) : 0;}

    inline void set(uint32_t v) {m[v >> 6] |= 1ull << (v & 63);}
    inline uint32_t left(uint32_t v) const {return (v - 62) * 64;}
    inline uint32_t parent(uint32_t v) const {return v / 64 + 62;}
    inline void reset(uint32_t v) {m[v >> 6] &= ~(1ull << (v & 63));}
    inline uint64_t childs_value(uint32_t v) const {return m[left(v) >> 6];}
    inline uint32_t get(uint32_t v) const {return m[v >> 6] >> (v & 63) & 1;}

    inline uint32_t left_go(uint32_t x, const uint32_t c) const {
        const uint64_t rem = x & 63;
        uint32_t bt = PREF * 64 + x;
        uint64_t num = m[bt >> 6] & lowest_bits(rem + c);
        if (num) return (x ^ rem) | __lg(num);
        for (bt = parent(bt); bt > 62; bt = parent(bt)) {
            const uint64_t rem = bt & 63;
            num = m[bt >> 6] & lowest_bits(rem);
            if (num) {bt = (bt ^ rem) | __lg(num); break;}
        }
        if (bt == 62) return NO;
        while (bt < PREF * 64) bt = left(bt) | __lg(m[bt - 62]);
        return bt - PREF * 64;
    }

    inline uint32_t right_go(uint32_t x, const uint32_t c) const {
        const uint64_t rem = x & 63;
        uint32_t bt = PREF * 64 + x;
        uint64_t num = m[bt >> 6] & ~lowest_bits(rem + c);
        if (num) return (x ^ rem) | __builtin_ctzll(num);
        for (bt = parent(bt); bt > 62; bt = parent(bt)) {
            const uint64_t rem = bt & 63;
            num = m[bt >> 6] & ~lowest_bits(rem + 1);
            if (num) {bt = (bt ^ rem) | __builtin_ctzll(num); break;}
        }
        if (bt == 62) return NO;
        while (bt < PREF * 64) bt = left(bt) | __builtin_ctzll(m[bt - 62]);
        return bt - PREF * 64;
    }

public:
    intset_faster() {set(62);}

    void insert(uint32_t x) {
        for (uint32_t v = PREF * 64 + x; !get(v); v = parent(v)) set(v);
    }

    void erase(uint32_t x) {
        if (!get(PREF * 64 + x)) return;
        reset(PREF * 64 + x);
        for (uint32_t v = parent(PREF * 64 + x); v > 62 && !childs_value(v); v = parent(v)) reset(v);
    }

    bool empty() const {return get(63);}
    void clear() {memset(m, 0, sizeof(m[0]) * SZ); set(62);}
    bool count(uint32_t x) const {return m[PREF + (x >> 6)] >> (x & 63) & 1;}

    uint32_t lower_bound(uint32_t x) const {return right_go(x, 0);}
    uint32_t upper_bound(uint32_t x) const {return right_go(x, 1);}
    uint32_t inverse_lower_bound(uint32_t x) const {return left_go(x, 1);}
    uint32_t inverse_upper_bound(uint32_t x) const {return left_go(x, 0);}
};
