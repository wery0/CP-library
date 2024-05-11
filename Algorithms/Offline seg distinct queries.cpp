class offline_seg_distinct_queries {

    int n;
    vector<int> closest_left_eq;
    vector<array<int, 3>> store;

public:
    template<typename T>
    offline_seg_distinct_queries(vector<T>& arr, size_t Q = 0): n(arr.size()), closest_left_eq(n, -1) {
        store.reserve(Q);
        umap<T, int> mp;
        for (size_t i = 0; i < n; ++i) {
            if (mp.count(arr[i])) closest_left_eq[i] = mp[arr[i]];
            mp[arr[i]] = i;
        }
    }

    void add_query(size_t l, size_t r) {
        assert(l <= r && r < n);
        using T = int;
        store.push_back({T(l), T(r), T(store.size())});
    }

    void clear() {
        store.clear();
    }

    //O(Qlog(n))
    vector<int> process_queries() {
        const size_t Q = store.size();
        vector<int> ans(Q);
        sort(store.begin(), store.end(), [&](const auto& l, const auto& r) {
            return l[1] < r[1];
        });
        vector<int> fen(n + 1);
        for (size_t r = 0, i = 0; r < n; ++r) {
            for (size_t p = r + 1; p <= n; p += p & -p) ++fen[p];
            if (closest_left_eq[r] != -1) {
                for (size_t p = closest_left_eq[r] + 1; p <= n; p += p & -p) --fen[p];
            }
            for (; i < Q && store[i][1] == r; ++i) {
                auto [l, r, u] = store[i];
                int res = 0;
                for (size_t p = r + 1; p; p -= p & -p) res += fen[p];
                for (size_t p = l; p; p -= p & -p) res -= fen[p];
                ans[u] = l > r ? 0 : res;
            }
        }
        return ans;
    }
};
