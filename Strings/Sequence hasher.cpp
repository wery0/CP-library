namespace sequence_hasher {

    uniform_int_distribution<uint64_t> gen(0, numeric_limits<uint64_t>::max());
    mt19937_64 rndll(timeStamp().time_since_epoch().count());

    template<typename I>
    __uint128_t hash_sequence_2_128_sum(I f, I l, const int64_t p = 1837174919) {
        static const __uint128_t kek = (((__uint128_t)gen(rndll)) << 64) | gen(rndll);
        __uint128_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p) o += (*f ^ kek) * pw;
        return o;
    }

    template<typename I>
    __uint128_t hash_sequence_2_128_xor(I f, I l, const int64_t p = 1837174919) {
        static const __uint128_t kek = (((__uint128_t)gen(rndll)) << 64) | gen(rndll);
        __uint128_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p) o ^= (*f ^ kek) * pw;
        return o;
    }

    template<typename I>
    __int128_t hash_sequence_128(I f, I l, __int128_t mod, const __int128_t p = 1837174919) {
        __int128_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p % mod) o += (*f) * pw, o %= mod;
        return o;
    }

    template<typename I>
    uint64_t hash_sequence_2_64_sum(I f, I l, const int64_t p = 1837174919) {
        static const uint64_t kek = gen(rndll);
        uint64_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p) o += (*f ^ kek) * pw;
        return o;
    }

    template<typename I>
    uint64_t hash_sequence_2_64_xor(I f, I l, const int64_t p = 1837174919) {
        static const uint64_t kek = gen(rndll);
        uint64_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p) o ^= (*f ^ kek) * pw;
        return o;
    }

    template<typename I>
    int64_t hash_sequence_64(I f, I l, int64_t mod, const int64_t p = 1837174919) {
        int64_t o = 0, pw = 1;
        for (; f != l; ++f, pw = pw * p % mod) o += (*f) * pw, o %= mod;
        return o;
    }
}
