template<typename T>
struct static_range_frequency {

    umap<T, int> mp;
    vec<vec<int>> store;

    static_range_frequency() = default;

    template<typename Iterator>
    static_range_frequency(Iterator first, Iterator last) {
        //using T = typename iterator_traits<Iterator>::value_type;
        int a = last - first;
        mp = umap<T, int>(a);
        vec<int> m(a);
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
        store = vec<vec<int>>(df, vec<int>());
        for (int q = 0; q < a; ++q) {
            store[m[q]].pb(q);
        }
    }

    int query(int l, int r, T val) {
        int n = mp.count(val) ? mp[val] : -1;
        if (n == -1) return 0;
        return upper_bound(all(store[n]), r) - lower_bound(all(store[n]), l);
    }
};
//change hash map if need.
//static_range_frequency<_Type> srf;
//static_range_frequency<_Type> srf(all(_arr));