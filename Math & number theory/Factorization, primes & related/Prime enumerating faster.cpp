//Calls function f() for each prime in range [L, R]
//<O(sqrt(R)loglog(sqrt(R)) + (R - L)loglog(R)), O(sqrt(R))> with a good constant
void enumerate_primes(uint64_t l, uint64_t r, auto f) {
    if (r < 2 || l > r) return;
    const uint64_t lo_u = max<uint64_t>(l, 2ull);
    static constexpr uint32_t blk = 30030 * 64;
    static constexpr uint32_t bn = blk / 128;
    static constexpr size_t sieve_b_words = 40500;
    static constexpr size_t sieve_tmp_words = 50500;
    #define get(B, x) ((B)[(x) >> 6] & (1ull << ((x) & 63)))
    #define add(B, x) ((B)[(x) >> 6] |= (1ull << ((x) & 63)))
    vector<uint64_t> b(sieve_b_words), tmp(sieve_tmp_words);
    uint64_t* pb = b.data();
    uint64_t* ptmp = tmp.data();
    vector<uint32_t> p = {0}, stk;
    auto sieve_one_block = [&](uint64_t sL, uint64_t sR) {
        memcpy(pb, ptmp, sieve_b_words * sizeof(uint64_t));
        uint32_t j;
        uint64_t st1, st7, st11, st13, st17, st19, st23, st29, mn;
        for (j = 7; j < (uint32_t)p.size() && 1ull * p[j] * p[j] <= sR; ++j) {
            if (1ull * p[j] * p[j] * p[j] >= sR) {
                for (; stk[j] < (uint32_t)p.size() && 1ull * p[j] * p[stk[j]] <= sR; ++stk[j]) {
                    if (1ull * p[j] * p[stk[j]] < sL) continue;
                    add(pb, (1ull * p[j] * p[stk[j]] - sL) >> 1);
                }
                continue;
            }
            const uint32_t w = p[j], w30 = w * 30;
            const uint64_t stu = max<uint64_t>(w, (sL + w - 1) / w);
            st1 = ((stu - 1 + 29) / 30) * 30 + 1;
            st7 = ((stu - 7 + 29) / 30) * 30 + 7;
            st11 = ((stu - 11 + 29) / 30) * 30 + 11;
            st13 = ((stu - 13 + 29) / 30) * 30 + 13;
            st17 = ((stu - 17 + 29) / 30) * 30 + 17;
            st19 = ((stu - 19 + 29) / 30) * 30 + 19;
            st23 = ((stu - 23 + 29) / 30) * 30 + 23;
            st29 = ((stu - 29 + 29) / 30) * 30 + 29;
            st1 = st1 * w - sL;
            st7 = st7 * w - sL;
            st11 = st11 * w - sL;
            st13 = st13 * w - sL;
            st17 = st17 * w - sL;
            st19 = st19 * w - sL;
            st23 = st23 * w - sL;
            st29 = st29 * w - sL;
            mn = min(min(min(st1, st7), min(st11, st13)), min(min(st17, st19), min(st23, st29)));
            while (mn < blk) {
#define ADD(B, x) add((B), (x) >> 1), add((B), ((x) + w30) >> 1), (x) += (w30 * 2)
                ADD(pb, st1);
                ADD(pb, st7);
                ADD(pb, st11);
                ADD(pb, st13);
                ADD(pb, st17);
                ADD(pb, st19);
                ADD(pb, st23);
                ADD(pb, st29);
#undef ADD
                mn += w30 * 2;
            }
        }
    };
    auto extend_primes_upto = [&](uint64_t lim_u) {
        if (lim_u <= blk) return;
        uint64_t L = blk + 1;
        if ((L & 1ull) == 0) ++L;
        while (L <= lim_u) {
            const uint64_t R = min(L + blk - 1, lim_u);
            stk.assign(p.size(), 0);
            for (size_t i = 1; i < p.size(); ++i) stk[i] = (uint32_t)i;
            sieve_one_block(L, R);
            for (uint32_t j = 0; j < bn; ++j) {
                uint64_t t1 = ~b[j];
                while (t1) {
                    uint32_t c = (uint32_t)__builtin_ctzll(t1);
                    uint64_t vu = L + ((uint64_t)j << 7) + (uint64_t)(c << 1);
                    if (vu <= R && vu <= lim_u) p.push_back((uint32_t)vu);
                    t1 &= t1 - 1;
                }
            }
            stk.assign(p.size(), 0);
            for (size_t i = 1; i < p.size(); ++i) stk[i] = (uint32_t)i;
            L = R + 1;
        }
    };
    uint32_t i, j;
    uint64_t st1, st7, st11, st13, st17, st19, st23, st29, mn;
    uint32_t v[] = {3, 5, 7, 11, 13};
    p.push_back(2);
    for (uint32_t I = 0; I < 5; ++I) {
        i = v[I];
        p.push_back(i);
        for (j = i; j <= blk; j += i * 2) {
            add(ptmp, (j - 1) >> 1);
            add(pb, (j - 1) >> 1);
        }
    }
    for (i = 3; i <= blk; i += 2) {
        if (!get(pb, (i - 1) >> 1)) {
            p.push_back(i);
            for (j = i; j <= blk; j += i * 2) add(pb, (j - 1) >> 1);
        }
    }
    stk.assign(p.size(), 0);
    for (i = 1; i < (uint32_t)p.size(); ++i) stk[i] = i;
    const uint64_t lim_u = max<uint64_t>(blk, sqrtl(r));
    extend_primes_upto(lim_u);
    stk.assign(p.size(), 0);
    for (i = 1; i < (uint32_t)p.size(); ++i) stk[i] = i;
    for (size_t idx = 1; idx < p.size() && (uint64_t)p[idx] <= r; ++idx) {
        const uint64_t pr = p[idx];
        if (pr >= lo_u) f(pr);
    }
    if (r <= lim_u) return;
    const uint64_t bi_hi = (r >= 1) ? (r - 1) / blk : 0;
    if (bi_hi == 0) return;
    for (uint64_t bi = 1; bi <= bi_hi; ++bi) {
        const uint64_t seg_l = bi * blk + 1;
        const uint64_t seg_r = seg_l + blk - 1;
        const bool intersects = (seg_r >= lo_u && seg_l <= r);
        if (!intersects) {
            for (j = 7; j < (uint32_t)p.size() && 1ull * p[j] * p[j] <= seg_r; ++j) {
                if (1ull * p[j] * p[j] * p[j] >= seg_r) {
                    for (; stk[j] < (uint32_t)p.size() && 1ull * p[j] * p[stk[j]] <= seg_r; ++stk[j]) {}
                }
            }
            continue;
        }

        memcpy(pb, ptmp, sieve_b_words * sizeof(uint64_t));
        for (j = 7; j < (uint32_t)p.size() && 1ull * p[j] * p[j] <= seg_r; ++j) {
            if (1ull * p[j] * p[j] * p[j] >= seg_r) {
                for (; stk[j] < (uint32_t)p.size() && 1ull * p[j] * p[stk[j]] <= seg_r; ++stk[j]) {
                    if (1ull * p[j] * p[stk[j]] < seg_l) continue;
                    add(pb, (1ull * p[j] * p[stk[j]] - seg_l) >> 1);
                }
                continue;
            }
            const uint32_t w = p[j], w30 = w * 30;
            const uint64_t stu = max<uint64_t>(w, (seg_l + w - 1) / w);
            st1 = ((stu - 1 + 29) / 30) * 30 + 1;
            st7 = ((stu - 7 + 29) / 30) * 30 + 7;
            st11 = ((stu - 11 + 29) / 30) * 30 + 11;
            st13 = ((stu - 13 + 29) / 30) * 30 + 13;
            st17 = ((stu - 17 + 29) / 30) * 30 + 17;
            st19 = ((stu - 19 + 29) / 30) * 30 + 19;
            st23 = ((stu - 23 + 29) / 30) * 30 + 23;
            st29 = ((stu - 29 + 29) / 30) * 30 + 29;
            st1 = st1 * w - seg_l;
            st7 = st7 * w - seg_l;
            st11 = st11 * w - seg_l;
            st13 = st13 * w - seg_l;
            st17 = st17 * w - seg_l;
            st19 = st19 * w - seg_l;
            st23 = st23 * w - seg_l;
            st29 = st29 * w - seg_l;
            mn = min(min(min(st1, st7), min(st11, st13)), min(min(st17, st19), min(st23, st29)));
            while (mn < blk) {
#define ADD(B, x) add((B), (x) >> 1), add((B), ((x) + w30) >> 1), (x) += (w30 * 2)
                ADD(pb, st1);
                ADD(pb, st7);
                ADD(pb, st11);
                ADD(pb, st13);
                ADD(pb, st17);
                ADD(pb, st19);
                ADD(pb, st23);
                ADD(pb, st29);
#undef ADD
                mn += w30 * 2;
            }
        }
        for (j = 0; j < bn; ++j) {
            uint64_t t1 = ~b[j];
            while (t1) {
                const uint64_t kk = ((__builtin_ctzll(t1) << 1) | ((uint64_t)j << 7)) + seg_l;
                if (kk > r) {
                    t1 -= (-t1 & t1);
                    continue;
                }
                if (intersects && kk > lim_u && kk >= lo_u) f(kk);
                t1 -= (-t1 & t1);
            }
            if ((((uint64_t)j + 1) << 7) + seg_l > r) break;
        }
    }
}
