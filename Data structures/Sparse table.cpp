template<typename T>
class sparse_table {
    size_t n, K;
    vector<vector<T>> m;

    //Change, if need
    inline T merge(T x, T y) const {
        return min(x, y);
    }

public:
    template<typename I>
    sparse_table(I first, I last) : n(last - first), K(__lg(n) + 1), m(K) {
        for (size_t i = 0; i < K; ++i) m[i].resize(max((size_t)0, n - (1 << i) + 1));
        copy(first, last, m[0].begin());
        for (size_t i = 1; i < K; ++i) {
            for (size_t j = 0; j + (1 << i) <= n; ++j) {
                m[i][j] = merge(m[i - 1][j], m[i - 1][j + (1 << (i - 1))]);
            }
        }
    }
    template<typename T_arr>
    sparse_table(T_arr& arr) {*this = sparse_table(arr.begin(), arr.end());}

    T query(size_t l, size_t r) const {
        const int u = __lg(r - l + 1);
        return merge(m[u][l], m[u][r - (1 << u) + 1]);
    }
};