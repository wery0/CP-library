template<typename K, typename V>
struct hash_map_open_addressing {

    size_t N;
    vector<pair<K, V>> store;
    vector<bool> is_occupied;

    hash_map_open_addressing(size_t N = 1): N(N), store(N), is_occupied(N) {assert(N > 0);}

    constexpr inline void next_pos(int& pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
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
        fill(is_occupied.begin(), is_occupied.end(), false);
    }

    V operator[](const K& key) const {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return store[pos].second;
            next_pos(pos);
        }
        return V();
    }

    bool contains(const K& key) const {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return true;
            next_pos(pos);
        }
        return false;
    }

    void add(const K& key, const V& val) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) {
                store[pos].second = val;
                return;
            }
            next_pos(pos);
        }
        store[pos] = {key, val};
        is_occupied[pos] = true;
    }
};