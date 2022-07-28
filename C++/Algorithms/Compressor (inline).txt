namespace compressor {
    template<class T>
    struct is_pair : std::false_type {};

    template<class T1, class T2>
    struct is_pair<std::pair<T1, T2> > : std::true_type {};

    template<class T>
    struct is_pair_d : is_pair<typename std::decay<T>::type> {};

    template<typename K, const K first_val, typename H>
    void unit_compress(H &h, vec<K> &store) {
        if constexpr(is_fundamental<H>::value) {
            h = first_val + lower_bound(all(store), h) - store.begin();
        } else if constexpr(is_pair_d<decltype(h)>::value) {
            unit_compress<K, first_val>(h.F, store);
            unit_compress<K, first_val>(h.S, store);
        } else {
            auto it = h.begin();
            while (it != h.end()) {
                unit_compress<K, first_val>(*it, store);
                ++it;
            }
        }
    }
    template<typename K, const K first_val, typename H, typename... T>
    void do_compress(vec<K> &store, H &h, T &... t) {
        unit_compress<K, first_val> (h, store);
        if constexpr(sizeof...(t)) do_compress<K, first_val>(store, t...);
    }

    template<typename K, typename H>
    void extract(H &h, vec<K> &store) {
        if constexpr(is_fundamental<H>::value) store.pb(h);
        else if constexpr(is_pair_d<decltype(h)>::value) {
            extract(h.F, store);
            extract(h.S, store);
        } else {
            auto it = h.begin();
            while (it != h.end()) {
                extract(*it, store);
                ++it;
            }
        }
    }
    template<typename K, typename H, typename... T>
    void extract_data(vec<K> &store, H &h, T &... t) {
        extract(h, store);
        if constexpr(sizeof...(t)) extract_data(store, t...);
    }

    template<typename K, const K first_val, typename H, typename... T>
    void inline_compress(H &h, T &... t) {
        vec<K> store;
        extract_data(store, h, t...);
        unify(store);
        do_compress<K, first_val>(store, h, t...);
    }
}
//usage: compressor::inline_compress<value_type, first_val>(args)