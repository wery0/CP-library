template<typename T>
struct rsq2D {
    int n, m;
    vector<vector<T>> store;

    rsq2D() = default;

    rsq2D(vector<vector<T>> &arr) {
        n = arr.size();
        m = n ? arr[0].size() : 0;
        store.resize(n + 1, vector<T>(m + 1));
        for (int q = 0; q < n; ++q) {
            for (int w = 0; w < m; ++w) {
                store[q + 1][w + 1] = arr[q][w] + store[q + 1][w] + store[q][w + 1] - store[q][w];
            }
        }
    }

    T query(int x1, int y1, int x2, int y2) {
        // assert(0 <= x1 && x1 < n);assert(0 <= x2 && x2 < n);assert(0 <= y1 && y1 < m);assert(0 <= y2 && y2 < m);
        // if (x1 > x2) swap(x1, x2);if (y1 > y2) swap(y1, y2);
        return store[x2 + 1][y2 + 1] - store[x1][y2 + 1] - store[x2 + 1][y1] + store[x1][y1];
    }
};
