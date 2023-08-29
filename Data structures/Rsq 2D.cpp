template<typename T>
class rsq2D {
    size_t n, m;
    vector<vector<T>> store;

public:
    rsq2D() = default;

    template<typename U>
    rsq2D(vector<vector<U>>& arr): n(arr.size()), m(n ? arr[0].size() : 0) {
        store.resize(n + 1, vector<T>(m + 1));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                store[i + 1][j + 1] = arr[i][j] + store[i + 1][j] + store[i][j + 1] - store[i][j];
            }
        }
    }

    T query(size_t x1, size_t y1, size_t x2, size_t y2) {
        // assert(max(x1, x2) < n); assert(max(y1, y2) < m);
        // if (x1 > x2) swap(x1, x2);if (y1 > y2) swap(y1, y2);
        return store[x2 + 1][y2 + 1] - store[x1][y2 + 1] - store[x2 + 1][y1] + store[x1][y1];
    }
};
