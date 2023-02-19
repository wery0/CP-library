template<const ll MOD, const ll P>
struct hasher {
    size_t a;
    vector<ll> pref_hash;
    vector<ll> pows;

    hasher() = default;

    template<typename Iterator>
    hasher(Iterator first, Iterator last): a(last - first), pref_hash(a), pows(a) {
        using T = typename iterator_traits<Iterator>::value_type;
        pows[0] = 1;
        pref_hash[0] = hash<T>{}(*first); ++first;
        for (size_t q = 1; q < a; ++q, ++first) {
            pows[q] = pows[q - 1] * P % MOD;
            pref_hash[q] = (pref_hash[q - 1] + hash<T>{}(*first) * pows[q]) % MOD;
        }
    }

    ll calc_hash(int l, int r) {
        ll o = pref_hash[r] - (l ? pref_hash[l - 1] : 0);
        if (o < 0) o += MOD;
        return o * pows[a - 1 - r] % MOD;
    }

    bool is_substrings_equal(int l1, int r1, int l2, int r2) {
        if (r1 - l1 != r2 - l2) return 0;
        return calc_hash(l1, r1) == calc_hash(l2, r2);
    }
};

struct multiple_hasher {
    static constexpr int SZ = 2;
    static constexpr ll mod[SZ] = {1'000'000'009, 2'000'000'011};
    static constexpr ll p[SZ] = {666'667, 997};
    hasher<mod[0], p[0]> h0;
    hasher<mod[1], p[1]> h1;

    multiple_hasher() = default;

    template<typename Iterator>
    multiple_hasher(Iterator first, Iterator last) {
        h0 = hasher<mod[0], p[0]>(first, last);
        h1 = hasher<mod[1], p[1]>(first, last);
    }

    bool is_substrings_equal(int l1, int r1, int l2, int r2) {
        return h0.is_substrings_equal(l1, r1, l2, r2) &&
               h1.is_substrings_equal(l1, r1, l2, r2);
    }

    array<ll, SZ> calc_hash(int l, int r) {
        return {h0.calc_hash(l, r), h1.calc_hash(l, r)};
    }
};