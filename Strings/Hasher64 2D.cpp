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
template<const uint64_t MOD = 4000000000000000037, const uint64_t P1 = 2807516534892679321, const uint64_t P2 = 2000000000000000057>
struct hasher64_2d {
    size_t n, m;
    vector<vector<uint64_t>> pref_hash;
    vector<uint64_t> pows1, pows2;

    //Works for <= 63 bit modulo
    //Change this function, if you need another way to multiply big numbers.
    static uint64_t mulmod(const uint64_t x, const uint64_t y) {
        uint64_t c = (long double)x * y / MOD;
        int64_t ans = int64_t(x * y - c * MOD) % int64_t(MOD);
        return ans < 0 ? ans + MOD : ans;
    }

    template<typename T>
    auto hash_elem(T x) {return hash<T>{}(x);}

public:
    hasher64_2d() = default;

    template<typename T_arr>
    hasher64_2d(vector<T_arr> v): n(v.size()), m(n ? v[0].size() : 0), pows1(n + 1), pows2(m + 1) {
        pref_hash.resize(n, vector<uint64_t>(m));
        pows1[0] = pows2[0] = 1;
        if (!n || !m) return;
        for (size_t i = 1; i <= n; ++i) pows1[i] = mulmod(pows1[i - 1], P1);
        for (size_t i = 1; i <= m; ++i) pows2[i] = mulmod(pows2[i - 1], P2);
        pref_hash[0][0] = hash_elem(v[0][0]);
        for (size_t j = 1; j < m; ++j) {
            pref_hash[0][j] = pref_hash[0][j - 1] + mulmod(hash_elem(v[0][j]), pows2[j]);
            pref_hash[0][j] -= pref_hash[0][j] < MOD ? 0 : MOD;
        }
        for (size_t i = 1; i < n; ++i) {
            pref_hash[i][0] = pref_hash[i - 1][0] + mulmod(hash_elem(v[i][0]), pows1[i]);
            pref_hash[i][0] -= pref_hash[i][0] < MOD ? 0 : MOD;
            for (size_t j = 1; j < m; ++j) {
                pref_hash[i][j] = mulmod(mulmod(hash_elem(v[i][j]), pows1[i]), pows2[j]) + pref_hash[i - 1][j];
                pref_hash[i][j] -= pref_hash[i][j] < MOD ? 0 : MOD;
                pref_hash[i][j] += pref_hash[i][j - 1];
                pref_hash[i][j] -= pref_hash[i][j] < MOD ? 0 : MOD;
                pref_hash[i][j] += pref_hash[i][j] < pref_hash[i - 1][j - 1] ? MOD : 0;
                pref_hash[i][j] -= pref_hash[i - 1][j - 1];
            }
        }
    }

    uint64_t rect_hash(size_t x1, size_t y1, size_t x2, size_t y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        assert(x2 < n && y2 < m);
        uint64_t o = pref_hash[x2][y2];
        if (x1) o += o < pref_hash[x1 - 1][y2] ? MOD : 0, o -= pref_hash[x1 - 1][y2];
        if (y1) o += o < pref_hash[x2][y1 - 1] ? MOD : 0, o -= pref_hash[x2][y1 - 1];
        if (x1 && y1) o += pref_hash[x1 - 1][y1 - 1], o -= o < MOD ? 0 : MOD;
        return mulmod(mulmod(o, pows1[n - x1]), pows2[m - y1]);
    }
};
