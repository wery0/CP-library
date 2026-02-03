namespace compressor {
    template<class T> struct is_pair: std::false_type {};
    template<class T1, class T2> struct is_pair<std::pair<T1, T2> >: std::true_type {};
    template<class T> struct is_pair_d: is_pair<typename std::decay<T>::type> {};

    template<typename K, const K first_val, typename H>
    void unit_compress(H& h, vector<K>& store) {
        if constexpr(is_fundamental<H>::value) {
            h = first_val + lower_bound(store.begin(), store.end(), h) - store.begin();
        } else if constexpr(is_pair_d<H>::value) {
            unit_compress<K, first_val>(h.first, store);
            unit_compress<K, first_val>(h.second, store);
        } else {
            auto it = h.begin();
            while (it != h.end()) {
                unit_compress<K, first_val>(*it, store);
                ++it;
            }
        }
    }
    template<typename K, const K first_val, typename H, typename... T>
    void do_compress(vector<K>& store, H& h, T&... t) {
        unit_compress<K, first_val> (h, store);
        if constexpr(sizeof...(t)) do_compress<K, first_val>(store, t...);
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

    template<typename K, const K first_val, typename H, typename... T>
    void inline_compress(H& h, T&... t) {
        vector<K> store;
        extract_data(store, h, t...);
        sort(store.begin(), store.end());
        store.erase(unique(store.begin(), store.end()), store.end());
        do_compress<K, first_val>(store, h, t...);
    }
}
//Usage: compressor::inline_compress<value_type, first_val>(args)
