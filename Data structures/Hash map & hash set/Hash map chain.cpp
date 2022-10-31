template<typename K, typename V>
struct hash_map_chain {

    int N;
    vec<vec<pair<K, V>>> table;

    hash_map_chain() = default;

    hash_map_chain(int n) {
        N = n;
        table.resize(n);
    }

    const ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral<K>::value) {
            //return key % N;
            return ((key * kek) >> 32) % N;
        } else {
            return hash<K> {}(key) % N;
        }
    }

    inline void clear() {
        for (int q = 0; q < N; ++q) {
            table[q].clear();
            table[q].shrink_to_fit();
        }
    }

    inline V operator[](const K& key) const {
        for (const auto &[k, v] : table[hsh(key)]) {
            if (k == key) return v;
        }
        return 0;
    }

    inline bool contains(const K& key) const {
        for (const auto &[k, v] : table[hsh(key)]) {
            if (k == key) return true;
        }
        return false;
    }

    inline void add(const K& key, const V& val) {
        int pos = hsh(key);
        for (auto &[k, v] : table[pos]) {
            if (k == key) {
                v = val;
                return;
            }
        }
        table[pos].pb({key, val});
    }

    inline void add_unchecked(const K& key, const V& val) {
        table[hsh(key)].pb({key, val});
    }
};