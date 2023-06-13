//Farach-Colton and Bender (FCB) algorithm variation for static RMQ queries
//O(N)* preprocessing, O(1) query, but on practice performs about the same as bottom-up segtree until N <= 5e7
//Set max_queries = true for RmaxQ, false for RminQ
//*: assuming that N <= 2 ^ WORD_SIZE
template<typename T, const bool max_queries>
class FCB {
    struct sparse_table {
        size_t n, K;
        vector<vector<T>> m;

        inline T merge(T x, T y) const {
            if constexpr(max_queries) return max(x, y);
            else return min(x, y);
        }

        sparse_table() = default;
        template<typename I>
        sparse_table(I first, I last): n(last - first), K(n ? __lg(n) + 1 : 0), m(K) {
            if (!n) return;
            for (size_t i = 0; i < K; ++i) m[i].resize(max((size_t)0, n - (1 << i) + 1));
            copy(first, last, m[0].begin());
            for (size_t i = 1; i < K; ++i) {
                for (size_t j = 0; j + (1 << i) <= n; ++j) {
                    m[i][j] = merge(m[i - 1][j], m[i - 1][j + (1 << (i - 1))]);
                }
            }
        }

        T query(size_t l, size_t r) const {
            const int u = __lg(r - l + 1);
            return merge(m[u][l], m[u][r - (1 << u) + 1]);
        }
    };

    static constexpr uint32_t highest_bits[] = {4294967295, 4294967294, 4294967292, 4294967288, 4294967280, 4294967264, 4294967232, 4294967168, 4294967040, 4294966784, 4294966272, 4294965248, 4294963200, 4294959104, 4294950912, 4294934528, 4294901760, 4294836224, 4294705152, 4294443008, 4293918720, 4292870144, 4290772992, 4286578688, 4278190080, 4261412864, 4227858432, 4160749568, 4026531840, 3758096384, 3221225472, 2147483648};
    size_t n;
    vector<T> m;
    vector<uint32_t> stacks;
    sparse_table table;

    void init() {
        n = m.size();
        vector<T> blocks_extreme(n >> 5);
        for (size_t i = 0; i + 32 <= n; i += 32) {
            if constexpr(max_queries) blocks_extreme[i >> 5] = *max_element(m.begin() + i, m.begin() + i + 32);
            else blocks_extreme[i >> 5] = *min_element(m.begin() + i, m.begin() + i + 32);
        }
        table = sparse_table(blocks_extreme.begin(), blocks_extreme.end());
        stacks.resize(n);
        for (size_t i = 0; i < n; i += 32) {
            uint32_t stack = 0;
            const size_t gr = min((size_t)32, n - i);
            for (size_t j = 0; j < gr; ++j) {
                if constexpr(max_queries) while (stack && m[i + j] >= m[i + __lg(stack)]) stack ^= 1 << __lg(stack);
                else while (stack && m[i + j] <= m[i + __lg(stack)]) stack ^= 1 << __lg(stack);
                stack |= (uint32_t)1 << j;
                stacks[i + j] = stack;
            }
        }
    }

public:
    FCB() = default;
    template<typename I>
    FCB(I first, I last): m(first, last) {init();}
    template<typename T_arr>
    FCB(const T_arr& arr) {*this = FCB(arr.begin(), arr.end());}
    FCB(vector<T>&& arr): m(move(arr)) {init();}

    T query(size_t l, size_t r) const {
        size_t bl = l >> 5;
        size_t br = r >> 5;
        if (bl == br) return m[(l & ~31) + __builtin_ctz(stacks[r] & highest_bits[l & 31])];
        T lf = m[(l & ~31) + __builtin_ctz(stacks[l | 31] & highest_bits[l & 31])];
        T rg = m[(r & ~31) + __builtin_ctz(stacks[r])];
        if (br - bl == 1) {
            if constexpr(max_queries) return max(lf, rg);
            else return min(lf, rg);
        }
        if constexpr(max_queries) return max({lf, table.query(bl + 1, br - 1), rg});
        else return min({lf, table.query(bl + 1, br - 1), rg});
    }
};