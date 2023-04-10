template<typename K, typename V>
struct hash_map_chain {

    size_t N;
    vector<vector<pair<K, V>>> table;

    hash_map_chain() = default;

    hash_map_chain(size_t N): N(N), table(N) {
        //for (auto& v : table) v.reserve(5);
    }

    static constexpr uint64_t kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral_v<K>) {
            //return key % N;
            return ((key * kek) >> 32) % N;
        } else {
            return hash<K>{}(key) % N;
        }
    }

    void clear() {
        for (auto& v : table) v.clear(), v.shrink_to_fit();
    }

    V operator[](const K& key) const {
        for (const auto& [k, v] : table[hsh(key)]) {
            if (k == key) return v;
        }
        return 0;
    }

    bool contains(const K& key) const {
        for (const auto& [k, v] : table[hsh(key)]) {
            if (k == key) return true;
        }
        return V();
    }

    void add(const K& key, const V& val) {
        int pos = hsh(key);
        for (auto& [k, v] : table[pos]) {
            if (k == key) {
                v = val;
                return;
            }
        }
        table[pos].emplace_back(key, val);
    }

    void add_unchecked(const K& key, const V& val) {
        table[hsh(key)].emplace_back(key, val);
    }
};