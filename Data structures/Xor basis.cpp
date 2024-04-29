//T - integral type we are working with.
//N - max bitness of numbers, i. e. all numbers are in range [0, 2^N).
//O(N * sizeof(T)) memory.
template<typename T, const size_t N>
class xor_basis {
    static_assert(is_integral_v<T>);
    static_assert(is_unsigned_v<T>);
    static_assert(N <= sizeof(T) * 8);

    static constexpr T MAX_VAL = numeric_limits<T>::max() >> (sizeof(T) * 8 - N);

    T r = 0;
    T a[N];

public:
    xor_basis() = default;
    template<typename I>
    xor_basis(I first, I last) {
        for (; first != last; ++first) insert(*first);
    }

    //O(1)
    int rank() const {return r;}
    bool empty() const {return r == 0;}
    void clear() {r = 0;}
    auto begin() const {return a;}
    auto end() const {return a + r;}

    //O(r)
    void insert(T val) {
        assert(val <= MAX_VAL);
        for (T b : *this) val = min(val, val ^ b);
        if (val == 0) return;
        a[r] = val;
        for (size_t i = r++; i && a[i - 1] < a[i]; --i) swap(a[i - 1], a[i]);
    }
    bool contains(T x) const {
        return calc_min_xor(x) == 0;
    }
    T calc_min_xor(T x) const {
        for (T b : *this) x = min(x, x ^ b);
        return x;
    }
    T calc_max_xor(T x) const {
        for (T b : *this) x = max(x, x ^ b);
        return x;
    }
    vector<T> get_basis() const {
        vector<T> res(r);
        std::copy(a, a + r, res.begin());
        return res;
    }

    //O((lhs.r + rhs.r) ^ 2)
    void merge_with(const xor_basis& rhs) {
        for (T b : rhs) insert(b);
    }
    //Zassenhaus algorithm
    template<typename H>
    void intersect_with(const xor_basis& rhs) {
        static_assert(sizeof(H) * 8 >= N * 2);
        xor_basis<H, N * 2> nb;
        for (H b : *this) nb.insert(b << N | b);
        for (H b : rhs) nb.insert(b << N);
        for (r = 0; H b : nb) {
            if (b < H(1) << N) a[r++] = b;
        }       
    }
};
using basis = xor_basis<uint32_t, 30>;
