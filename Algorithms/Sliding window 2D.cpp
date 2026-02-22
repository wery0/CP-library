//Moves between all submatrixes a * b in matrix n * m
//O((n - a) * (m - b) * min(a, b) + nm) calls of add() and rem()
template<typename T>
vector<vector<T>> sliding_window_2d(vector<vector<T>>& t, size_t a, size_t b, auto add, auto rem, auto query) {
    size_t n = t.size(), m = n ? t[0].size() : 0;
    assert(0 < a && a <= n);
    assert(0 < b && b <= m);
    vector res(n - a + 1, vector<T>(m - b + 1));
    for (int x = 0; x < a; ++x) for (int y = 0; y < b; ++y) add(t[x][y]);
    for (int i = 0, j = 0, fl = a < b, dx = !fl, dy = fl, cc = (n - a + 1) * (m - b + 1); ; i += dx, j += dy) {
        if (res[i][j] = query(); !--cc) break;
        if (int ni = i + dx, nj = j + dy; fl) {
            if (dx == 1 || nj < 0 || nj >= m - b + 1) dy ? dx = 1, dy = 0 : (dx = 0, dy = j ? -1 : 1);
        } else {
            if (dy == 1 || ni < 0 || ni >= n - a + 1) dx ? dx = 0, dy = 1 : (dx = i ? -1 : 1, dy = 0);
        }
        if (dy == 1) {
            for (int x = i, y = j + b; x < i + a; ++x) add(t[x][y]);
            for (int x = i, y = j; x < i + a; ++x) rem(t[x][y]);
        } else if (dy == -1) {
            for (int x = i, y = j + b - 1; x < i + a; ++x) rem(t[x][y]);
            for (int x = i, y = j - 1; x < i + a; ++x) add(t[x][y]);
        } else if (dx == 1){
            for (int x = i, y = j; y < j + b; ++y) rem(t[x][y]);
            for (int x = i + a, y = j; y < j + b; ++y) add(t[x][y]);
        } else {
            for (int x = i - 1, y = j; y < j + b; ++y) add(t[x][y]);
            for (int x = i + a - 1, y = j; y < j + b; ++y) rem(t[x][y]);
        }
    }
    return res;
}
