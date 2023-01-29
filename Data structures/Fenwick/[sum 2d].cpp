template<typename T>
class fenwick {
    int n, m;
    vector<vector<T>> fen;

public:
    fenwick() = default;
    fenwick(int n, int m): n(n + 1), m(m + 1), fen(n + 1, vector<T>(m + 1)) {}
    //O(nm) initialization
    template<typename U>
    fenwick(vector<vector<U>>& v): n(v.size() + 1), m(n ? v[0].size() + 1 : 1), fen(n, vector<T>(m)) {
        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < m; ++j) {
                const int nx = i + (i & -i);
                const int ny = j + (j & -j);
                fen[i][j] += v[i - 1][j - 1];
                if (nx < n) fen[nx][j] += fen[i][j];
                if (ny < m) fen[i][ny] += fen[i][j];
                if (nx < n && ny < m) fen[nx][ny] -= fen[i][j];
            }
        }
    }

    void clear() {
        for (auto& arr : fen) {
            fill(arr.begin(), arr.end(), 0);
        }
    }

    void point_add(int x, int y, T val) {
        ++x, ++y;
        assert(1 <= x && x < n); assert(1 <= y && y < m);
        for (; x < n; x += x & -x) {
            for (int cpy = y; cpy < m; cpy += cpy & -cpy) {
                fen[x][cpy] += val;
            }
        }
    }

    //Sum on [0, x] x [0, y]
    T pref_sum(int x, int y) const {
        ++x, ++y;
        assert(0 <= x && x < n); assert(0 <= y && y < m);
        T o = 0;
        for (; x; x -= x & -x) {
            for (int cpy = y; cpy; cpy -= cpy & -cpy) {
                o += fen[x][cpy];
            }
        }
        return o;
    }

    //Sum on [x, n - 1] x [y, m - 1]
    T suf_sum(int x, int y) const {
        return rect_sum(x, y, n - 2, m - 2);
    }

    //Sum on [x1, x2] x [y1, y2]
    T rect_sum(int x1, int y1, int x2, int y2) const {
        assert(0 <= x1 && x2 < n - 1); assert(0 <= y1 && y2 < m - 1); assert(x1 <= x2 && y1 <= y2);
        return pref_sum(x2, y2) - pref_sum(x2, y1 - 1) -
               pref_sum(x1 - 1, y2) + pref_sum(x1 - 1, y1 - 1);
    }
};
