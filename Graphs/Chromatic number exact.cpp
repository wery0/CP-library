template<const size_t N>
class ChromaticNumberExact {

    size_t n;
    vector<vector<int>> g;

    static auto time_stamp() {return std::chrono::steady_clock::now();}
    static auto duration_milli(auto a) {return chrono::duration_cast<chrono::milliseconds>(a).count();}

public:
    ChromaticNumberExact(size_t n): n(n), g(n) {
        assert(n <= N);
    }

    void add_edge(size_t x, size_t y) {
        assert(x != y);
        assert(max(x, y) < n);
        g[x].push_back(y);
        g[y].push_back(x);
    }

    void clear() {
        for (auto& l : g) l.clear(); 
    }

    //Calculates exact chromatic number using recursion with cutoffs
    //If it doesn't finish, set positive time limit
    pair<int, vector<int>> calc_chromatic_number_and_coloring(const double TL = 0) const {
        auto start_time = time_stamp();
        int best = n + 1, die = 0;
        vector<int> best_col;
        function<void(int, int)> go = [&](int mx, int eso) {
            static uint8_t cnt = 0;
            static vector<int> col(n);
            if (die || mx >= best) return;
            die |= !++cnt && TL > 0 && duration_milli(time_stamp() - start_time) / 1e3 > TL;
            if (eso == 0) {
                best = mx, best_col = col;
                return;
            }
            int mxb = -1, v;
            bitset<N> mask, maskv;
            for (int i = 0; i < n; ++i) {
                if (col[i]) continue;
                mask.reset();
                for (int h : g[i]) mask[col[h]] = 1;
                int r = mask.count() - mask[0];
                if (r > mxb) mxb = r, v = i, maskv = mask;
            }
            for (int c = 1; c < mx + 2; ++c) {
                if (maskv[c]) continue;
                col[v] = c;
                go(max(mx, c), eso - 1);
            }
            col[v] = 0;
        };
        go(0, n);
        return {best, best_col};
    }
};
