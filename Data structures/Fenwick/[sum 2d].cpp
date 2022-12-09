template<typename T>
struct fenwick {
    int N, M;
    vector<vector<T>> fen;

    fenwick() = default;
    fenwick(int N, int M): N(N + 1), M(M + 1), fen(N + 1, vector<T>(M + 1)) {}

    //O(NM) initialization
    template<typename U>
    fenwick(vector<vector<U>> &n): N(n.size() + 1), M(N ? n[0].size() + 1 : 1), fen(N, vector<T>(M)) {
        for (int q = 1; q <= n.size(); q++) {
            for (int w = 1; w <= n[0].size(); w++) {
                const int nx = q + (q & -q);
                const int ny = w + (w & -w);
                fen[q][w] += n[q - 1][w - 1];
                if (nx < N) fen[nx][w] += fen[q][w];
                if (ny < M) fen[q][ny] += fen[q][w];
                if (nx < N && ny < M) fen[nx][ny] -= fen[q][w];
            }
        }
        for (int q = n.size() + 1; q < N; q++) {
            for (int w = n[0].size() + 1; w < M; w++) {
                const int nx = q + (q & -q);
                const int ny = w + (w & -w);
                if (nx < N) fen[nx][w] += fen[q][w];
                if (ny < M) fen[q][ny] += fen[q][w];
                if (nx < N && ny < M) fen[nx][ny] -= fen[q][w];
            }
        }
    }

    void clear() {
        for (int q = 0; q < N; ++q) {
            fill(fen[q].begin(), fen[q].end(), 0);
        }
    }

    void point_add(int x, int y, T val) {
        assert(0 <= x && x < N - 1); assert(0 <= y && y < M - 1);
        for (x += 1; x < N; x += x & -x) {
            for (int cpy = y + 1; cpy < M; cpy += cpy & -cpy) {
                fen[x][cpy] += val;
            }
        }
    }

    //Sum on [0, x] x [0, y]
    T pref_sum(int x, int y) {
        assert(-1 <= x && x < N - 1); assert(-1 <= y && y < M - 1);
        T o = 0;
        for (x += 1; x; x -= x & -x) {
            for (int cpy = y + 1; cpy; cpy -= cpy & -cpy) {
                o += fen[x][cpy];
            }
        }
        return o;
    }

    //Sum on [x, N - 1] x [y, M - 1]
    T suf_sum(int x, int y) {
        return rect_sum(x, y, N - 2, M - 2);
    }

    //Sum on [x1, x2] x [y1, y2]
    T rect_sum(int x1, int y1, int x2, int y2) {
        assert(0 <= x1 && x2 < N - 1); assert(0 <= y1 && y2 < M - 1); assert(x1 <= x2 && y1 <= y2);
        return pref_sum(x2, y2) - pref_sum(x2, y1 - 1) -
               pref_sum(x1 - 1, y2) + pref_sum(x1 - 1, y1 - 1);
    }
};