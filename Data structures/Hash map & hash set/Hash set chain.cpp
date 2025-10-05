template<typename K>
struct hash_set_chain {

    size_t N;
    vector<vector<K>> table;

    static constexpr uint64_t kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral<K>) {
            //return key % N;
            return ((key * kek) >> 32) % N;
        } else {
            return hash<K>{}(key) % N;
        }
    }

    hash_set_chain() = default;

    hash_set_chain(size_t N): N(N), table(N) {
        //for (vector<K>& v : table) v.reserve(5);
    }

    bool contains(const K& key) const {
        int pos = hsh(key);
        return any_of(table[pos].begin(), table[pos].end(), [&](const K& k) {return k == key;});
    }

    void insert(const K& key) {
        int pos = hsh(key);
        if (!any_of(table[pos].begin(), table[pos].end(), [&](const K& k) {return k == key;})) {
            table[pos].push_back(key);
        }
    }

    void insert_unchecked(const K& key) {
        table[hsh(key)].push_back(key);
    }
};
