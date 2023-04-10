template<typename K>
struct hash_set_open_addressing {

    size_t N;
    vector<K> store;
    vector<bool> is_occupied;

    hash_set_open_addressing(size_t N = 1): N(N), store(N), is_occupied(N) {assert(N > 0);}

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

    bool contains(const K& key) const {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos] == key) return true;
            next_pos(pos);
        }
        return false;
    }

    void insert(const K& key) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos] == key) return;
            next_pos(pos);
        }
        store[pos] = key;
        is_occupied[pos] = true;
    }
};