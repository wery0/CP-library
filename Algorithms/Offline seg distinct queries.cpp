class offline_seg_distinct_queries {

    int n;
    vector<int> closest_left_eq;

    template<typename I>
    vector<int> get_closest_left_eq(I first, I last) {
        using T = typename iterator_traits<I>::value_type;
        const int n = last - first;
        vector<int> ans(n, -1);
        umap<T, int> mp;
        auto cit = first;
        for (int q = 0; q < n; ++q, ++cit) {
            T cur = *cit;
            if (mp.count(cur)) ans[q] = mp[cur];
            mp[cur] = q;
        }
        return ans;
    }

public:
    template<typename T>
    offline_seg_distinct_queries(vector<T>& arr): n(arr.size()) {
        closest_left_eq = get_closest_left_eq(arr.begin(), arr.end());
    }

    vector<int> process_queries(vector<pair<int, int>> queries) {
        int z = queries.size();
        vector<int> ans(z);
        vector<array<int, 3>> queriess(z);
        for (int q = 0; q < z; ++q) queriess[q] = {queries[q].first, queries[q].second, q};
        sort(queriess.begin(), queriess.end(), [&](const auto& l, const auto& r) {
            return l[1] < r[1];
        });
        vector<int> fen(n + 1);
        for (int r = 0, i = 0; r < n; r++) {
            for (int p = r + 1; p <= n; p += p & -p) ++fen[p];
            if (closest_left_eq[r] != -1) {
                for (int p = closest_left_eq[r] + 1; p <= n; p += p & -p) --fen[p];
            }
            for (; i < z && queriess[i][1] == r; ++i) {
                int sum = 0;
                for (int p = queriess[i][1] + 1; p; p -= p & -p) sum += fen[p];
                for (int p = queriess[i][0]; p; p -= p & -p) sum -= fen[p];
                ans[queriess[i][2]] = sum;
            }
        }
        return ans;
    }
};