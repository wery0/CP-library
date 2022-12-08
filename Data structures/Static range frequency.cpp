template<typename T>
struct static_range_frequency {

    umap<T, int> mp;
    vector<vector<int>> store;

    static_range_frequency() = default;

    template<typename Iterator>
    static_range_frequency(Iterator first, Iterator last) {
        int a = last - first;
        mp.reserve(a);
        vector<int> m(a);
        int df = 0;
        for (int q = 0; q < a; ++q, ++first) {
            int n = mp.count(*first) ? mp[*first] : -1;
            if (n == -1) {
                mp[*first] = df;
                m[q] = df++;
            } else {
                m[q] = n;
            }
        }
        store.resize(df);
        for (int q = 0; q < a; ++q) {
            store[m[q]].push_back(q);
        }
    }

    int query(int l, int r, T val) {
        int n = mp.count(val) ? mp[val] : -1;
        if (n == -1) return 0;
        return upper_bound(all(store[n]), r) - lower_bound(all(store[n]), l);
    }
};
//Change hash map if need.
//static_range_frequency<_Type> srf;
//static_range_frequency<_Type> srf(all(_arr));