template<typename T>
class sparse_table {
    int K, G;
    vector<vector<T>> m;

    template<typename I>
    void init_table(I fir, I last) {
        G = last - fir;
        K = __lg(G) + 1;
        m.resize(K);
        for (int q = 0; q < K; q++) m[q].resize(max(0, G - (1 << q) + 1));
        for (int q = 0; q < G; q++) m[0][q] = *(fir + q);
        for (int q = 1; q < K; q++) {
            for (int w = 0; w + (1 << q) <= G; w++) {
                m[q][w] = merge(m[q - 1][w], m[q - 1][w + (1 << (q - 1))]);
            }
        }
    }

    //Change, if need
    inline T merge(T x, T y) {
        return min(x, y);
    }

public:
    template<typename T_arr>
    sparse_table(T_arr& n) {init_table(all(n));}
    template<typename I>
    sparse_table(I fir, I last) {init_table(fir, last);}

    T query(int l, int r) {
        int u = __lg(r - l + 1);
        return merge(m[u][l], m[u][r - (1 << u) + 1]);
    }
};