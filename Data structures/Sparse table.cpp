//Binary function merge() must be associative, commutative and idempotent
//<O(nlog(n)T(merge)), O(nlog(n))> preprocessing, O(T(merge)) per query
template<typename T, auto merge = [](const T& l, const T& r) -> T{return min(l, r);}>
class sparse_table {
    size_t n, K;
    vector<vector<T>> m;

public:
    sparse_table() = default;
    template<typename I>
    sparse_table(I first, I last): n(std::distance(first, last)), K(n ? __lg(n) + 1 : 0), m(K) {
        if (!n) return;
        for (size_t i = 0; i < K; ++i) m[i].resize(max<size_t>(0, n + 1 - (1 << i)));
        std::copy(first, last, m[0].begin());
        for (size_t i = 1; i < K; ++i) {
            for (size_t j = 0; j + (1 << i) <= n; ++j) {
                m[i][j] = merge(m[i - 1][j], m[i - 1][j + (1 << (i - 1))]);
            }
        }
    }
    template<typename T_arr>
    sparse_table(const T_arr& arr) {*this = sparse_table(arr.begin(), arr.end());}

    T query(size_t l, size_t r) const {
        assert(l <= r && r < n);
        const int u = __lg(r - l + 1);
        return merge(m[u][l], m[u][r - (1 << u) + 1]);
    }
};
