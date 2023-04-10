template<typename K, typename V, const int LG>
struct hash_map_open_addressing {

    const int N = 1 << LG;
    vector<pair<K, V>> store = vector<pair<K, V>>(N);
    vector<bool> is_occupied = vector<bool>(N);

    constexpr inline void next_pos(int& pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    static constexpr ull kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(is_integral_v<K>) {
            //return key & (N - 1);
            return (key * kek) >> (64 - LG);
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