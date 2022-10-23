template<typename K, const int LG>
struct hash_set_open_addressing {

    const int N = 1 << LG;
    vec<K> store = vec<K>(N);
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

    inline bool contains(const K& key) const {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos] == key) return true;
        }
        return false;
    }

    inline void insert(const K& key) {
        int pos = hsh(key);
        for (; is_occupied[pos]; next_pos(pos)) {
            if (store[pos] == key) {
                return;
            }
        }
        store[pos] = key;
        is_occupied[pos] = true;
    }
};