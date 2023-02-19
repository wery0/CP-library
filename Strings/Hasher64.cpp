//This hasher can work with modules of order 1e18.
/*Some big primes:
666667
1000000000000000003 = 1e18 + 3
1000000000000777771 = 1e18 + 777771
2000000000000000057 = 2e18 + 57
2000000000000000137 = 2e18 + 137
4000000000000000037 = 4e18 + 37
2807516534892679321
3835424442118071511
*/
template<const uint64_t MOD, const uint64_t P>
class hasher64 {
    size_t n;
    vector<uint64_t> pref_hash;
    vector<uint64_t> pows;

    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    uint64_t big_prod_mod(const uint64_t x, const uint64_t y, const uint64_t mod) {
        uint64_t c = (long double)x * y / mod;
        int64_t ans = int64_t(x * y - c * mod) % int64_t(mod);
        return ans < 0 ? ans + mod : ans;
    }

public:
    hasher64() = default;

    template<typename Iterator>
    hasher64(Iterator first, Iterator last): n(last - first), pref_hash(n), pows(n) {
        using T = typename iterator_traits<Iterator>::value_type;
        pows[0] = 1;
        pref_hash[0] = hash<T>{}(*first); ++first;
        for (int q = 1; q < n; ++q, ++first) {
            pows[q] = big_prod_mod(pows[q - 1], P, MOD);
            pref_hash[q] = pref_hash[q - 1];
            pref_hash[q] += big_prod_mod(hash<T>{}(*first), pows[q], MOD);
            pref_hash[q] -= pref_hash[q] >= MOD ? MOD : 0;
        }
    }

    uint64_t calc_hash(int l, int r) {
        uint64_t o = pref_hash[r];
        if (o < (l ? pref_hash[l - 1] : 0)) o += MOD;
        o -= (l ? pref_hash[l - 1] : 0);
        return big_prod_mod(o, pows[n - 1 - r], MOD);
    }

    bool is_substrings_equal(int l1, int r1, int l2, int r2) {
        if (r1 - l1 != r2 - l2) return 0;
        return calc_hash(l1, r1) == calc_hash(l2, r2);
    }
};