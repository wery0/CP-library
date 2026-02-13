/*
Some big primes:
666667
1000000000000000003 = 1e18 + 3
1000000000000777771 = 1e18 + 777771
2000000000000000057 = 2e18 + 57
2000000000000000137 = 2e18 + 137
4000000000000000037 = 4e18 + 37
2807516534892679321
3835424442118071511
*/
//This polynomial hasher can work with modules of order 1e18.
//Uncomment commented lines to return real hash, rather then multiplied by P ^ n. Requires P to be invertible by MOD.
template<const uint64_t MOD = 3835424442118071511, const uint64_t P = 2807516534892679321>
struct hasher64 {
    size_t n;
    vector<uint64_t> pref_hash;
    vector<uint64_t> pows;
    // vector<uint64_t> ipows;


    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    static uint64_t mulmod(const uint64_t x, const uint64_t y) {
        uint64_t c = (long double)x * y / MOD;
        int64_t ans = int64_t(x * y - c * MOD) % int64_t(MOD);
        return ans < 0 ? ans + MOD : ans;
    }

    template<typename T>
    auto hash_elem(T x) {return hash<T> {}(x);}

    static uint64_t binpow(uint64_t x, uint64_t k) {
        uint64_t o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = mulmod(o, x);
            x = mulmod(x, x);
        }
        return o;
    }

public:
    hasher64() = default;

    template<typename Iterator>
    hasher64(Iterator first, Iterator last): n(std::distance(first, last)), pref_hash(n), pows(n + 1) {
        pows[0] = 1;
        if (!n) return;
        pref_hash[0] = hash_elem(*first); ++first;
        for (size_t i = 1; i < n; ++i, ++first) {
            pows[i] = mulmod(pows[i - 1], P);
            pref_hash[i] = pref_hash[i - 1] + mulmod(hash_elem(*first), pows[i]);
            pref_hash[i] -= pref_hash[i] < MOD ? 0 : MOD;
        }
        pows[n] = mulmod(pows[n - 1], P);
        // ipows.resize(n + 1);
        // ipows[n] = binpow(pows[n], MOD - 2);
        // for (ssize_t i = n - 1; i >= 0; --i) {
        //     ipows[i] = mulmod(ipows[i + 1], P);
        // }
    }

    //Returns hash of string l + r, where hash(l) = hl, hash(r) = hr, len(l) = len_l
    //O(1) if len_l <= n, O(log(len_l)) otherwise
    uint64_t merge_hashes(uint64_t len_l, uint64_t hl, uint64_t hr) const {
        uint64_t pw = len_l < pows.size() ? pows[len_l] : binpow(P, len_l);
        uint64_t hlr = hl + mulmod(hr, pw);
        return hlr < MOD ? hlr : hlr - MOD;
    }

    //O(1)
    uint64_t seg_hash(size_t l, size_t r) const {
        if (l > r) return 0;
        assert(r < n);
        uint64_t o = pref_hash[r];
        if (l) {
            o += o < pref_hash[l - 1] ? MOD : 0;
            o -= pref_hash[l - 1];
        }
        return mulmod(o, pows[n - l]);
        // return mulmod(o, ipows[l]);
    }

    //Returns the hash of string s[l, r] * k = s[l, r] + ... + s[l, r] (k - 1 concatenations)
    //O(log(k))
    uint64_t seg_hash_repeated(size_t l, size_t r, uint64_t k) const {
        uint64_t ans = 0, anspw = 1;
        uint64_t hs = seg_hash(l, r), hspw = pows[r - l + 1];
        for (; k; k >>= 1) {
            if (k & 1) {
                ans += mulmod(hs, anspw);
                ans -= ans < MOD ? 0 : MOD;
                anspw = mulmod(anspw, hspw);
            }
            hs += mulmod(hs, hspw);
            hs -= hs < MOD ? 0 : MOD;
            hspw = mulmod(hspw, hspw);
        }
        return ans;
    }

    //O(|borders|)
    uint64_t calc_hash_of_substrings_concatenation(vector<pair<int, int>> borders) const {
        uint64_t res = 0;
        for (uint64_t cpw = 1; auto [l, r] : borders) {
            if (l > r) continue;
            res += mulmod(seg_hash(l, r), cpw);
            res -= res < MOD ? 0 : MOD;
            cpw = mulmod(cpw, pows[r - l + 1]);
        }
        return res;
    }

    //Returns length of longest common prefix of suffixes s[i, n - 1] and s[j, n - 1]
    //O(log(n))
    size_t lcp(size_t i, size_t j, size_t max_len = -2) const {
        if (i > j) swap(i, j);
        assert(j < n);
        size_t l = 0, r = min(max_len, n - j) + 1;
        if (seg_hash(i, i) != seg_hash(j, j)) return 0;
        if (seg_hash(i, i + r - 2) == seg_hash(j, j + r - 2)) return r - 1;
        while (l + 1 < r) {
            size_t m = l + (r - l) / 2;
            (seg_hash(i, i + m - 1) == seg_hash(j, j + m - 1) ? l : r) = m;
        }
        return l;
    }

    //Returns length of longest common suffix of prefixes s[0, i] and s[0, j]
    //O(log(n))
    size_t lcs(size_t i, size_t j, size_t max_len = -2) const {
        if (i > j) swap(i, j);
        assert(j < n);
        size_t l = 0, r = min(max_len + 1, i + 2);
        if (seg_hash(i, i) != seg_hash(j, j)) return 0;
        if (seg_hash(i - r + 2, i) == seg_hash(j - r + 2, j)) return r - 1;
        while (l + 1 < r) {
            size_t m = l + (r - l) / 2;
            (seg_hash(i + 1 - m, i) == seg_hash(j + 1 - m, j) ? l : r) = m;
        }
        return l;
    }
};
