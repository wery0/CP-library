// A, B must be at least 1 greater than the corresponding dimensions size
template<typename T>
struct fenwick {
    int A, B;
    vec<vec<T>> fen;

    fenwick() {}

    fenwick(int _A, int _B) {
        A = _A, B = _B;
        fen = vec<vec<T>>(A, vec<T>(B));
    }

    fenwick(vec<vec<T>> &n) {
        A = n.size() + 1;
        B = n.size() ? n[0].size() + 1 : 1;
        fen = vec<vec<T>>(A, vec<T>(B));
        for (int q = 1; q <= n.size(); q++) {
            for (int w = 1; w <= n[0].size(); w++) {
                const int nx = q + (q & -q);
                const int ny = w + (w & -w);
                fen[q][w] += n[q - 1][w - 1];
                if (nx < A) fen[nx][w] += fen[q][w];
                if (ny < B) fen[q][ny] += fen[q][w];
                if (nx < A && ny < B) fen[nx][ny] -= fen[q][w];
            }
        }
        for (int q = n.size() + 1; q < A; q++) {
            for (int w = n[0].size() + 1; w < B; w++) {
                const int nx = q + (q & -q);
                const int ny = w + (w & -w);
                if (nx < A) fen[nx][w] += fen[q][w];
                if (ny < B) fen[q][ny] += fen[q][w];
                if (nx < A && ny < B) fen[nx][ny] -= fen[q][w];
            }
        }
    }

    void clear() {
        for (int q = 0; q < A; ++q) fill(all(fen[q]), 0);
    }

    void point_add(int px, int py, T x) {
        assert(0 <= px && px < A - 1); assert(0 <= py && py < B - 1);
        for (px += 1; px < A; px += px & -px) {
            for (int cpy = py + 1; cpy < B; cpy += cpy & -cpy) {
                fen[px][cpy] += x;
            }
        }
    }

    T pref_sum(int px, int py) {
        assert(-1 <= px && px < A - 1); assert(-1 <= py && py < B - 1);
        T o = 0;
        for (px += 1; px; px -= px & -px) {
            for (int cpy = py + 1; cpy; cpy -= cpy & -cpy) {
                o += fen[px][cpy];
            }
        }
        return o;
    }

    T suf_sum(int px, int py) {
        return rect_sum(px, py, A - 2, B - 2);
    }

    T rect_sum(int x1, int y1, int x2, int y2) {
        assert(0 <= x1 && x2 < A - 1); assert(0 <= y1 && y2 < B - 1); assert(x1 <= x2 && y1 <= y2);
        return pref_sum(x2, y2) - pref_sum(x2, y1 - 1) -
               pref_sum(x1 - 1, y2) + pref_sum(x1 - 1, y1 - 1);
    }
};