constexpr ull lowest_bitsll[] = {0ull, 1ull, 3ull, 7ull, 15ull, 31ull, 63ull, 127ull, 255ull, 511ull, 1023ull, 2047ull, 4095ull, 8191ull, 16383ull, 32767ull, 65535ull, 131071ull, 262143ull, 524287ull, 1048575ull, 2097151ull, 4194303ull, 8388607ull, 16777215ull, 33554431ull, 67108863ull, 134217727ull, 268435455ull, 536870911ull, 1073741823ull, 2147483647ull, 4294967295ull, 8589934591ull, 17179869183ull, 34359738367ull, 68719476735ull, 137438953471ull, 274877906943ull, 549755813887ull, 1099511627775ull, 2199023255551ull, 4398046511103ull, 8796093022207ull, 17592186044415ull, 35184372088831ull, 70368744177663ull, 140737488355327ull, 281474976710655ull, 562949953421311ull, 1125899906842623ull, 2251799813685247ull, 4503599627370495ull, 9007199254740991ull, 18014398509481983ull, 36028797018963967ull, 72057594037927935ull, 144115188075855871ull, 288230376151711743ull, 576460752303423487ull, 1152921504606846975ull, 2305843009213693951ull, 4611686018427387903ull, 9223372036854775807ull, 18446744073709551615ull};
const uint NO = UINT32_MAX;   //This value will be returned in lower_bound functions, if no answer exists. Change, if need.
template<uint MAXN>           //Can correctly work with numbers in range [0; MAXN]
class godgod_suc_pred {
    static const uint PREF = (MAXN <= 64 ? 0 :
                              MAXN <= 4096 ? 1 :
                              MAXN <= 262144 ? 1 + 64 :
                              MAXN <= 16777216 ? 1 + 64 + 4096 :
                              MAXN <= 1073741824 ? 1 + 64 + 4096 + 262144 : 227) + 1;
    static const uint SZ = PREF + (MAXN + 63) / 64 + 1;
    ull m[SZ] = {0};

    inline uint left(uint v) const {return (v - 62) * 64;}
    inline uint parent(uint v) const {return v / 64 + 62;}
    inline void setbit(uint v) {m[v >> 6] |= 1ull << (v & 63);}
    inline void resetbit(uint v) {m[v >> 6] &= ~(1ull << (v & 63));}
    inline uint getbit(uint v) const {return m[v >> 6] >> (v & 63) & 1;}
    inline ull childs_value(uint v) const {return m[left(v) >> 6];}

    inline uint left_go(uint x, const uint c) const {
        const ull rem = x & 63;
        uint bt = PREF * 64 + x;
        ull num = m[bt >> 6] & lowest_bitsll[rem + c];
        if (num) return (x ^ rem) | __lg(num);
        for (bt = parent(bt); bt > 62; bt = parent(bt)) {
            const ull rem = bt & 63;
            num = m[bt >> 6] & lowest_bitsll[rem];
            if (num) {bt = (bt ^ rem) | __lg(num); break;}
        }
        if (bt == 62) return NO;
        while (bt < PREF * 64) bt = left(bt) | __lg(m[bt - 62]);
        return bt - PREF * 64;
    }

    inline uint right_go(uint x, const uint c) const {
        const ull rem = x & 63;
        uint bt = PREF * 64 + x;
        ull num = m[bt >> 6] & ~lowest_bitsll[rem + c];
        if (num) return (x ^ rem) | __builtin_ctzll(num);
        for (bt = parent(bt); bt > 62; bt = parent(bt)) {
            const ull rem = bt & 63;
            num = m[bt >> 6] & ~lowest_bitsll[rem + 1];
            if (num) {bt = (bt ^ rem) | __builtin_ctzll(num); break;}
        }
        if (bt == 62) return NO;
        while (bt < PREF * 64) bt = left(bt) | __builtin_ctzll(m[bt - 62]);
        return bt - PREF * 64;
    }

public:
    godgod_suc_pred() {
        assert(PREF != 228);
        setbit(62);
    }

    void clear() {fill(m, m + SZ, 0); setbit(62);}

    bool count(uint x) const {
        return m[PREF + (x >> 6)] >> (x & 63) & 1;
    }

    void insert(uint x) {
        for (uint v = PREF * 64 + x; !getbit(v); v = parent(v)) {
            setbit(v);
        }
    }

    void erase(uint x) {
        if (!getbit(PREF * 64 + x)) return;
        resetbit(PREF * 64 + x);
        for (uint v = parent(PREF * 64 + x); v > 62 && !childs_value(v); v = parent(v)) {
            resetbit(v);
        }
    }

    uint lower_bound(uint x) const {return right_go(x, 0);}
    uint upper_bound(uint x) const {return right_go(x, 1);}
    uint inverse_lower_bound(uint x) const {return left_go(x, 1);}
    uint inverse_upper_bound(uint x) const {return left_go(x, 0);}
};
//Supports all std::set operations in O(1) on random queries / dense arrays, O(log_64(N)) in worst case (sparce array).
//Count operation works in O(1) always.