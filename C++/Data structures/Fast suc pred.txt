constexpr ull lowest_bitsll[] = {0ull, 1ull, 3ull, 7ull, 15ull, 31ull, 63ull, 127ull, 255ull, 511ull, 1023ull, 2047ull, 4095ull, 8191ull, 16383ull, 32767ull, 65535ull, 131071ull, 262143ull, 524287ull, 1048575ull, 2097151ull, 4194303ull, 8388607ull, 16777215ull, 33554431ull, 67108863ull, 134217727ull, 268435455ull, 536870911ull, 1073741823ull, 2147483647ull, 4294967295ull, 8589934591ull, 17179869183ull, 34359738367ull, 68719476735ull, 137438953471ull, 274877906943ull, 549755813887ull, 1099511627775ull, 2199023255551ull, 4398046511103ull, 8796093022207ull, 17592186044415ull, 35184372088831ull, 70368744177663ull, 140737488355327ull, 281474976710655ull, 562949953421311ull, 1125899906842623ull, 2251799813685247ull, 4503599627370495ull, 9007199254740991ull, 18014398509481983ull, 36028797018963967ull, 72057594037927935ull, 144115188075855871ull, 288230376151711743ull, 576460752303423487ull, 1152921504606846975ull, 2305843009213693951ull, 4611686018427387903ull, 9223372036854775807ull, 18446744073709551615ull};
constexpr uint lowest_bits[] = {0u, 1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u, 511u, 1023u, 2047u, 4095u, 8191u, 16383u, 32767u, 65535u, 131071u, 262143u, 524287u, 1048575u, 2097151u, 4194303u, 8388607u, 16777215u, 33554431u, 67108863u, 134217727u, 268435455u, 536870911u, 1073741823u, 2147483647u, 4294967295u};
const uint NO = 10000;
template<uint MAXN>
struct god_suc_pred {
    static const uint SZ = (MAXN + 63) / 64;
    static const uint U = geq_pow2(SZ);
    bool kek[U * 2] = {0};
    ull m[SZ] = {0};

    god_suc_pred() {kek[0] = 1;}

    void operator=(god_suc_pred<MAXN> &other) {
        memcpy(kek, other.kek, sizeof(other.kek));
        memcpy(m, other.m, sizeof(other.m));
    }

    void clear() {fill(kek + 1, kek + U * 2, 0); fill(m, m + SZ, 0);}

    void add(uint p) {
        m[p >> 6] |= 1ull << (p & 63);
        for (p = (p >> 6) + U; !kek[p]; p >>= 1) {
            kek[p] = 1;
        }
    }

    void rem(uint p) {
        m[p >> 6] &= ~(1ull << (p & 63));
        p >>= 6;
        if (m[p]) return;
        for (p += U; p; p >>= 1) {
            kek[p] = 0;
            if (kek[p ^ 1]) break;
        }
    }

    bool contains(uint p) const {
        return m[p >> 6] >> (p & 63) & 1;
    }

    uint prev(uint x) const {
        const ull rem = x & 63;
        ull num = m[x >> 6] & lowest_bitsll[rem];
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

    uint next(uint x) const {
        const ull rem = x & 63;
        ull num = m[x >> 6] & ~lowest_bitsll[rem + 1];
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

    uint prev_inclusive(uint x) const {
        const ull rem = x & 63;
        ull num = m[x >> 6] & lowest_bitsll[rem + 1];
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

    uint next_inclusive(uint x) const {
        const ull rem = x & 63;
        ull num = m[x >> 6] & ~lowest_bitsll[rem];
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
};
//Supports add(x), remove(x), prev(x), next(x) operations
//Works in O(1) on random queries / dense arrays, O(log_2(N/64)) in worst case
//prev and next are non inclusive, i. e. [1, 5, 9] -> prev(5) = 1, next(5) = 9
//Can test it here: https://codeforces.com/gym/102591/problem/D
//Or here: https://judge.yosupo.jp/problem/predecessor_problem