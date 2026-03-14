namespace compressor {
    template<class T> struct is_pair: std::false_type {};
    template<class T1, class T2> struct is_pair<std::pair<T1, T2>>: std::true_type {};
    template<class T> struct is_pair_d: is_pair<typename std::decay<T>::type> {};

    template<typename K, typename H>
    void unit_compress(H& h, vector<K>& store) {
        if constexpr(is_fundamental<H>::value) {
            h = lower_bound(store.begin(), store.end(), h) - store.begin();
        } else if constexpr(is_pair_d<H>::value) {
            unit_compress<K>(h.first, store);
            unit_compress<K>(h.second, store);
        } else {
            auto it = h.begin();
            while (it != h.end()) {
                unit_compress<K>(*it, store);
                ++it;
            }
        }
    }
    template<typename K, typename H, typename... T>
    void do_compress(vector<K>& store, H& h, T&... t) {
        unit_compress<K> (h, store);
        if constexpr(sizeof...(t)) do_compress<K>(store, t...);
    }

    template<typename K, typename H>
    void extract(H& h, vector<K>& store) {
        if constexpr(is_fundamental<H>::value) store.push_back(h);
        else if constexpr(is_pair_d<decltype(h)>::value) {
            extract(h.first, store);
            extract(h.second, store);
        } else {
            auto it = h.begin();
            while (it != h.end()) {
                extract(*it, store);
                ++it;
            }
        }
    }

    template<typename K, typename H, typename... T>
    void extract_data(vector<K>& store, H& h, T&... t) {
        extract(h, store);
        if constexpr(sizeof...(t)) extract_data(store, t...);
    }

    template<typename K, typename H, typename... T>
    vector<K> inline_compress(H& h, T&... t) {
        vector<K> store;
        extract_data(store, h, t...);
        sort(store.begin(), store.end());
        store.erase(unique(store.begin(), store.end()), store.end());
        do_compress<K>(store, h, t...);
        return store;
    }
}
//Usage: auto store = compressor::inline_compress<value_type>(args);
