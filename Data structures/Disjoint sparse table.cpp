template<typename T>
class disjoint_sparse_table {
    int K, G;
    vector<vector<T>> m;

    template<typename I>
    void init_table(I fir, I last) {
        G = last - fir;
        K = __lg(G) + 1 + (!!(G & (G - 1)));
        m.resize(K - 1, vector<T>(G));
        for (int q = 1; q < K; q++) {
            for (int l = 0, md = (1 << q) / 2, r = (1 << q); md < G; l += 1 << q, md += 1 << q, r += 1 << q) {
                m[q - 1][md - 1] = *(fir + md - 1);
                m[q - 1][md] = *(fir + md);
                for (int w = md - 2; w >= l; w--) m[q - 1][w] = merge(m[q - 1][w + 1], *(fir + w));
                const int gr = min(G, r);
                for (int w = md + 1; w < gr; w++) m[q - 1][w] = merge(m[q - 1][w - 1], *(fir + w));
            }
        }
    }

    //Change, if need
    T merge(T x, T y) {
        return min(x, y);
    }

public:
    template<typename T_arr>
    disjoint_sparse_table(T_arr& n) {init_table(all(n));}
    template<typename I>
    disjoint_sparse_table(I fir, I last) {init_table(fir, last);}

    T query(int l, int r) {
        const int u = l == r ? 0 : __lg(l ^ r);
        return l == r ? m[0][l] : merge(m[u][l], m[u][r]);
    }
};