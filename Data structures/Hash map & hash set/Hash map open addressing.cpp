template<typename K, typename V>
struct hash_map_open_addressing {

    int N;
    vec<pair<K, V>> store;
    vec<bool> is_occupied;

    hash_map_open_addressing() = default;

    hash_map_open_addressing(int n) {
	assert(n > 0);
        N = n;
        store.resize(n);
        is_occupied.resize(n);
    }

    constexpr inline void next_pos(int &pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    const ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
    constexpr inline int hsh(const K& key) const {
        if constexpr(std::is_integral<K>::value) {
            //return key % N;
            return ((key * kek) >> 32) % N;
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
        is_occupied[pos] = true;
    }
};