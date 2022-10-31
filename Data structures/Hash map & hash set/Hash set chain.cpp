template<typename K>
struct hash_set_chain {

    int N;
    vec<vec<K>> table;

    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral<K>::value) {
            return key % N;
        } else {
            return hash<K> {}(key) % N;
        }
    }

    hash_set_chain() = default;

    hash_set_chain(int n) {
        N = n;
        table.resize(N);
        //for (int q = 0; q < N; ++q) table.reserve(5);
    }

    inline bool contains(const K& key) const {
        for (const auto &k : table[hsh(key)]) {
            if (k == key) return true;
        }
        return false;
    }

    inline void insert(const K& key) {
        int pos = hsh(key);
        for (const auto &k : table[pos]) {
            if (k == key) {
                return;
            }
        }
        table[pos].pb(key);
    }

    inline void insert_unchecked(const K& key) {
        table[hsh(key)].pb(key);
    }
};