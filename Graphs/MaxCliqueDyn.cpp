//Implementation of MaxCliqueDyn algorithm for finding max clique / max independent set
template<const size_t N>
struct MaxCliqueDyn {
    struct Vertex {int i, d = 0;};

    const double MAGIC = 0.025;

    size_t n;
    vector<bitset<N>> e;

    void init(vector<Vertex>& r) {
        for (auto& v : r) v.d = 0;
        for (auto& v : r) for (auto j : r) v.d += e[v.i][j.i];
        sort(r.begin(), r.end(), [](auto a, auto b) {return a.d > b.d;});
        for (int i = int(r.size()) - 1; i >= 0; --i) r[i].d = min(i, r[0].d) + 1;
    }

    vector<int> solve() {
        vector<Vertex> R;
        for (int i = 0; i < n; ++i) R.push_back({i});
        init(R);
        vector<vector<int>> C(n + 1);
        vector<int> qmax, q, S(n + 1), old(n + 1);
        double pk = 0;
        function<void(vector<Vertex>&, int)> go = [&](vector<Vertex>& R, int lev) {
            S[lev] += S[lev - 1] - old[lev];
            old[lev] = S[lev - 1];
            while (R.size()) {
                if (q.size() + R.back().d <= qmax.size()) return;
                q.push_back(R.back().i);
                vector<Vertex> T;
                for (auto v : R) if (e[R.back().i][v.i]) T.push_back({v.i});
                if (T.size()) {
                    if (S[lev]++ / ++pk < MAGIC) init(T);
                    int j = 0, mxk = 1, mnk = max<int>(qmax.size() - q.size() + 1, 1);
                    C[1].clear(), C[2].clear();
                    for (auto v : T) {
                        int k = 1;
                        while (any_of(C[k].begin(), C[k].end(), [&](int i) {return e[v.i][i];})) ++k;
                        if (k > mxk) mxk = k, C[mxk + 1].clear();
                        if (k < mnk) T[j++].i = v.i;
                        C[k].push_back(v.i);
                    }
                    if (j > 0) T[j - 1].d = 0;
                    for (int k = mnk; k <= mxk; ++k) {
                        for (int i : C[k]) T[j].i = i, T[j++].d = k;
                    }
                    go(T, lev + 1);
                } else if (q.size() > qmax.size()) {
                    qmax = q;
                }
                q.pop_back(), R.pop_back();
            }
        };
        go(R, 1);
        sort(qmax.begin(), qmax.end());
        return qmax;
    }

public:
    MaxCliqueDyn(size_t n): n(n), e(n) {}

    void add_edge(size_t x, size_t y) {
        assert(max(x, y) < n);
        e[x][y] = e[y][x] = 1;
    }

    void invert() {
        for (size_t i = 0; i < n; ++i) e[i] = ~e[i], e[i][i] = 0;
    }

    void clear() {
        for (size_t i = 0; i < n; ++i) e[i].reset();
    }

    vector<int> find_max_clique() {return solve();}
    vector<int> find_max_independent_set() {
        invert();
        auto res = find_max_clique();
        invert();
        return res;
    }
};
