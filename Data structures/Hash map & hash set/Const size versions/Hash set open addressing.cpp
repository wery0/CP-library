template<typename K, const int LG>
struct hash_set_open_addressing {

    const int N = 1 << LG;
    vector<K> store = vector<K>(N);
    vector<bool> is_occupied = vector<bool>(N);

    constexpr inline void next_pos(int& pos) const {
        pos = pos < N - 1 ? pos + 1 : 0;
    }

    static constexpr uint64_t kek = 11995408973635179863ull;
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