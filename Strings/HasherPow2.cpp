//This polynomial hasher efficiently uses modulo 2 ^ N. Fails on Thue-Morse strings, be careful.
//Uncomment commented lines to return real hash, rather then multiplied by P ^ n. Requires P to be odd.
template<size_t N, const uint64_t P = 2807516534892679321>
struct hasher_pow2 {
    static_assert(3 <= N && N <= 128);
    using T = conditional_t<N <= 8, uint8_t,
              conditional_t<N <= 16, uint16_t,
              conditional_t<N <= 32, uint32_t,
              conditional_t<N <= 64, uint64_t, 
              __uint128_t
              >>>>;
    static constexpr size_t uN = N & (N - 1) ? 2 << __lg(N) : N;
    static constexpr T MASK = ((T)-1) >> (uN - N);

    size_t n;
    vector<T> pref_hash;
    vector<T> pows;
    // vector<T> ipows;

    template<typename T>
    auto hash_elem(T x) {return hash<T> {}(x);}

    static T binpow(T x, T k) {
        T o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o *= x;
            x *= x;
        }
        return o;
    }

public:
    hasher_pow2() = default;

    template<typename Iterator>
    hasher_pow2(Iterator first, Iterator last): n(std::distance(first, last)), pref_hash(n), pows(n + 1) {
        pows[0] = 1;
        if (!n) return;
        pref_hash[0] = hash_elem(*first); ++first;
        for (size_t i = 1; i < n; ++i, ++first) {
            pows[i] = pows[i - 1] * P;
            pref_hash[i] = pref_hash[i - 1] + hash_elem(*first) * pows[i];
        }
        pows[n] = pows[n - 1] * P;
        // ipows.resize(n + 1);
        // ipows[n] = binpow(pows[n], MASK / 4);
        // for (ssize_t i = n - 1; i >= 0; --i) {
        //     ipows[i] = ipows[i + 1] * P;
        // }
    }

    //Returns hash of string l + r, where hash(l) = hl, hash(r) = hr, len(l) = len_l
    //O(1) if len_l <= n, O(log(len_l)) otherwise
    T merge_hashes(T len_l, T hl, T hr) const {
        T pw = len_l < pows.size() ? pows[len_l] : binpow(P, len_l);
        T hlr = hl + hr * pw;
        return hlr & MASK;
    }

    //O(1)
    T seg_hash(size_t l, size_t r) const {
        if (l > r) return 0;
        assert(r < n);
        T o = pref_hash[r] - (l ? pref_hash[l - 1] : 0);
        return o * pows[n - l] & MASK;
        // return o * ipows[l] & MASK;
    }

    //Returns the hash of string s[l, r] * k = s[l, r] + ... + s[l, r] (k - 1 concatenations)
    //O(log(k))
    T seg_hash_repeated(size_t l, size_t r, T k) const {
        T ans = 0, anspw = 1;
        T hs = seg_hash(l, r), hspw = pows[r - l + 1];
        for (; k; k >>= 1) {
            if (k & 1) {
                ans += hs * anspw;
                anspw *= hspw;
            }
            hs += hs * hspw;
            hspw *= hspw;
        }
        return ans & MASK;
    }

    //O(|borders|)
    T calc_hash_of_substrings_concatenation(vector<pair<int, int>> borders) const {
        T res = 0;
        for (T cpw = 1; auto [l, r] : borders) {
            if (l > r) continue;
            res += seg_hash(l, r) * cpw;
            cpw *= pows[r - l + 1];
        }
        return res & MASK;
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
