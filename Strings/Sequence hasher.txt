template<typename I>
__uint128_t hash_sequence_2_128_sum(I f, I l, const ll p = 1837174919) {
    static const __uint128_t kek = (((__uint128_t)uid<ull>(0, ULLONG_MAX)(rndll)) << 64) | uid<ull>(0, ULLONG_MAX)(rndll);
    __uint128_t o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p) o += (*f ^ kek) * pw;
    return o;
}

template<typename I>
__uint128_t hash_sequence_2_128_xor(I f, I l, const ll p = 1837174919) {
    static const __uint128_t kek = (((__uint128_t)uid<ull>(0, ULLONG_MAX)(rndll)) << 64) | uid<ull>(0, ULLONG_MAX)(rndll);
    __uint128_t o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p) o ^= (*f ^ kek) * pw;
    return o;
}

template<typename I>
ull hash_sequence_2_64_sum(I f, I l, const ll p = 1837174919) {
    static const ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
    ull o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p) o += (*f ^ kek) * pw;
    return o;
}

template<typename I>
ull hash_sequence_2_64_xor(I f, I l, const ll p = 1837174919) {
    static const ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
    ull o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p) o ^= (*f ^ kek) * pw;
    return o;
}

template<typename I>
ll hash_sequence(I f, I l, ll p, ll mod) {
    ll o = 0, pw = 1;
    for (; f != l; ++f, pw = pw * p % mod) o += (*f) * pw, o %= mod;
    return o;
}