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
template<const uint64_t MOD = 3835424442118071511, const uint64_t P = 2807516534892679321>
struct hasher64 {
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

    template<typename T>
    auto hash_elem(T x) {return hash<T>{}(x);}

public:
    hasher64() = default;

    template<typename Iterator>
    hasher64(Iterator first, Iterator last): n(last - first), pref_hash(n), pows(n + 1) {
        pows[0] = 1;
        if (!n) return;
        using T = typename iterator_traits<Iterator>::value_type;
        pref_hash[0] = hash_elem(*first); ++first;
        for (size_t i = 1; i < n; ++i, ++first) {
            pows[i] = big_prod_mod(pows[i - 1], P, MOD);
            pref_hash[i] = pref_hash[i - 1] + big_prod_mod(hash_elem(*first), pows[i], MOD);
            pref_hash[i] -= pref_hash[i] < MOD ? 0 : MOD;
        }
        pows[n] = big_prod_mod(pows[n - 1], P, MOD);
    }

    uint64_t calc_hash(int l, int r) {
        if (l > r) return 0;
        assert(0 <= l && r < n);
        uint64_t o = pref_hash[r];
        if (l) {
            o += o < pref_hash[l - 1] ? MOD : 0;
            o -= pref_hash[l - 1];
        }
        return big_prod_mod(o, pows[n - l], MOD);
    }

    //Returns the hash of string s[l, r] * k = s[l, r] + ... + s[l, r] (k - 1 concatenations)
    //O(log(k))
    uint64_t calc_hash_repeated(int l, int r, uint64_t k) {
        uint64_t ans = 0, anspw = 1;
        uint64_t hs = calc_hash(l, r), hspw = pows[r - l + 1];
        for (; k; k >>= 1) {
            if (k & 1) {
                ans += big_prod_mod(hs, anspw, MOD);
                ans -= ans < MOD ? 0 : MOD;
                anspw = big_prod_mod(anspw, hspw, MOD);
            }
            hs += big_prod_mod(hs, hspw, MOD);
            hs -= hs < MOD ? 0 : MOD;
            hspw = big_prod_mod(hspw, hspw, MOD);
        }
        return ans;
    }

    uint64_t calc_hash_of_substrings_concatenation(vector<pair<int, int>> borders) {
        uint64_t res = 0;
        for (uint64_t cpw = 1; auto [l, r] : borders) {
            if (l > r) continue;
            res += big_prod_mod(calc_hash(l, r), cpw, MOD);
            res -= res < MOD ? 0 : MOD;
            cpw = big_prod_mod(cpw, pows[r - l + 1], MOD);
        }
        return res;
    }
};