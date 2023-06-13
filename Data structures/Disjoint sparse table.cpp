template<typename T>
class disjoint_sparse_table {
    size_t n, K;
    vector<vector<T>> m;

    //Change, if need
    inline T merge(T x, T y) const {
        return min(x, y);
    }

public:
    disjoint_sparse_table() = default;
    template<typename I>
    disjoint_sparse_table(I first, I last): n(last - first), K(__lg(n) + 1 + (!!(n & (n - 1)))), m(K - 1, vector<T>(n)) {
        for (ssize_t i = 1; i < K; ++i) {
            for (ssize_t l = 0, md = (1 << i) / 2, r = 1 << i; md < n; l += 1 << i, md += 1 << i, r += 1 << i) {
                m[i - 1][md - 1] = *(first + md - 1);
                m[i - 1][md] = *(first + md);
                for (ssize_t j = md - 2; j >= l; --j) m[i - 1][j] = merge(m[i - 1][j + 1], *(first + j));
                for (ssize_t j = md + 1, gr = n < r ? n : r; j < gr; ++j) m[i - 1][j] = merge(m[i - 1][j - 1], *(first + j));
            }
        }
    }
    template<typename T_arr>
    disjoint_sparse_table(const T_arr& arr) {*this = disjoint_sparse_table(arr.begin(), arr.end());}

    T query(size_t l, size_t r) const {
        const int u = l == r ? 0 : __lg(l ^ r);
        return l == r ? m[0][l] : merge(m[u][l], m[u][r]);
    }
};