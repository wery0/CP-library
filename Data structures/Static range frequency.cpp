//Change hash map if need
template<typename T>
class static_range_frequency {

    umap<T, int> mp;
    vector<int> store;
    vector<int> cnt;

public:
    static_range_frequency() = default;

    template<typename Iterator>
    static_range_frequency(Iterator first, Iterator last) {
        const size_t n = last - first;
        mp.reserve(n);
        vector<int> m(n);
        int df = 0;
        for (size_t i = 0; i < n; ++i, ++first) {
            int& x = mp[*first];
            if (!x) x = ++df;
            m[i] = x;
        }
        cnt.resize(df + 2);
        for (T x : m) ++cnt[x + 1];
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        store.resize(n);
        for (size_t i = 0; i < n; ++i) {
            store[cnt[m[i]]++] = i;
        }
    }

    //Counts # occurrences of val in segment [l, r]
    //O(log(n))
    int query(size_t l, size_t r, T val) {
        int x = mp.count(val) ? mp[val] - 1 : -1;
        if (x == -1) return 0;
        return upper_bound(store.begin() + cnt[x], store.begin() + cnt[x + 1], r) -
               lower_bound(store.begin() + cnt[x], store.begin() + cnt[x + 1], l);
    }
};
