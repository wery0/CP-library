template<typename K, typename V, const int LG>
struct hash_map_open_addressing {

    const int N = 1 << LG;
    vec<pair<K, V>> store = vec<pair<K, V>>(N);
    vec<bool> is_occupied = vec<bool>(N);

    constexpr inline void next_pos(int &pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    static constexpr ull kek = 11995408973635179863ull;
    constexpr inline int hsh(const K& key) const {
        if constexpr(std::is_integral<K>::value) {
            //return key & (N - 1);
            return (key * kek) >> (64 - LG);
        } else {
            return hash<K> {}(key) % N;
        }
    }

    inline void clear() {
        fill(all(is_occupied), false);
    }

    inline V operator[](const K& key) const {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) return store[pos].S;
        }
        return 0;
    }

    inline bool contains(const K& key) const {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) return true;
        }
        return false;
    }

    inline void add(const K& key, const V& val) {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos].F == key) {
                store[pos].S = val;
                return;
            }
        }
        store[pos] = {key, val};
        is_occupied[pos] = 1;
    }
};