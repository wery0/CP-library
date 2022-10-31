template<typename T>
struct rsq2D {
    int a, b;
    vec<vec<T>> m;

    rsq2D() = default;

    rsq2D(vec<vec<T>> &_m) {
        a = _m.size();
        b = a ? _m[0].size() : 0;
        m = vec<vec<T>>(a + 1, vec<T>(b + 1));
        for (int q = 0; q < a; ++q) {
            for (int w = 0; w < b; ++w) {
                m[q + 1][w + 1] = _m[q][w] + m[q + 1][w] + m[q][w + 1] - m[q][w];
            }
        }
    }

    T query(int x1, int y1, int x2, int y2) {
        // assert(0 <= x1 && x1 < a);assert(0 <= x2 && x2 < a);assert(0 <= y1 && y1 < b);assert(0 <= y2 && y2 < b);
        // if (x1 > x2) swap(x1, x2);if (y1 > y2) swap(y1, y2);
        return m[x2 + 1][y2 + 1] - m[x1][y2 + 1] - m[x2 + 1][y1] + m[x1][y1];
    }
};