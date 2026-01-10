//Kuhn algorithm with optimizations and randomization.
class max_general_matching_random_kuhn {
    int n, tim = 0;
    vector<vector<int>> l;
    vector<int> mt, us;

    int dfs(int v) {
        static uniform_int_distribution<int> gen(2, 7);
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        us[v] = tim;
        shuffle(l[v].begin(), l[v].end(), rng);
        //rotate(l[v].begin(), l[v].begin() + l[v].size() / gen(rng), l[v].end());
        for (int h : l[v]) {
            int hm = mt[h];
            if (us[hm] != tim) {
                mt[v] = h, mt[h] = v, mt[hm] = n;
                if (hm == n || dfs(hm)) return 1;
                mt[h] = hm, mt[hm] = h, mt[v] = n;
            }
        }
        return 0;
    }

public:
    max_general_matching_random_kuhn() = default;
    max_general_matching_random_kuhn(int n): n(n), l(n), mt(n + 1, n), us(n + 1) {}

    void add_edge(int x, int y) {
        l[x].push_back(y);
        l[y].push_back(x);
    }

    const int MAGIC = 10;    //Enough for almost all graphs. Set some bigger value, if still WA.
    vector<pair<int, int>> calc() {
        int mt_size = 0;
        for (int no_change = 0; no_change < MAGIC; ++no_change) {
            int was = mt_size;
            for (int i = 0; i < n; ++i) {
                if (mt[i] == n) ++tim, mt_size += dfs(i);
            }
            if (mt_size != was) no_change = 0;
        }
        vector<pair<int, int>> ans;
        ans.reserve(mt_size);
        for (int i = 0; i < n; ++i) {
            if (mt[i] < i) ans.emplace_back(mt[i], i);
        }
        return ans;
    }
};
