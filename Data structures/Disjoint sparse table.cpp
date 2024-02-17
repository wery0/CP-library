//Binary function merge() must be associative and commutative
//<O(nlog(n)T(merge)), O(nlog(n))> preprocessing, O(T(merge)) per query
template<typename T, auto merge = [](const T& l, const T& r) -> T{return min(l, r);}>
class disjoint_sparse_table {
    size_t n, K;
    vector<vector<T>> m;
 
public:
    disjoint_sparse_table() = default;
    template<typename I>
    disjoint_sparse_table(I first, I last): n(std::distance(first, last)), K(n > 1 ? __lg(n) + 1 - !(n & (n - 1)) : n), m(K, vector<T>(n)) {
        auto get = [&](size_t i) {return *(first + i);};
        for (size_t i = 0; i < K; ++i) {
            std::copy(first, last, m[i].begin());
            if (i == 0) continue;
            const size_t step = 2 << i;
            for (size_t l = 0, md = step / 2 + 1; md <= n; l += step, md += step) {
                const size_t t = min(n + 1, md + step / 2);
                for (size_t j = md - 2; j > l; --j) m[i][j - 1] = merge(m[i][j], get(j - 1));
                for (size_t j = md + 1; j < t; ++j) m[i][j - 1] = merge(m[i][j - 2], get(j - 1));
            }
        }
    }
    template<typename T_arr>
    disjoint_sparse_table(const T_arr& arr) {*this = disjoint_sparse_table(arr.begin(), arr.end());}
 
    T query(size_t l, size_t r) const {
        assert(l <= r && r < n);
        const int u = l == r ? 0 : __lg(l ^ r);
        return l == r ? m[0][l] : merge(m[u][l], m[u][r]);
    }
};
 