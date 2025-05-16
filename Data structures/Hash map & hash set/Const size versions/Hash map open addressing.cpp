template<typename K, typename V, const int LG>
class hashmap_open_addressing {

    const size_t N = 1 << LG;
    size_t size_ = 0;
    vector<pair<K, V>> store = vector<pair<K, V>>(N);
    vector<char> is_occupied = vector<char>(N);

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

public:
    hashmap_open_addressing() = default;

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}
    size_t capacity() const {return N;}
    size_t count(const K& key) const {return contains(key);}

    void clear() {
        size_ = 0;
        fill(is_occupied.begin(), is_occupied.end(), false);
    }

    V& operator[](const K& key) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return store[pos].second;
            next_pos(pos);
        }
        ++size_;
        store[pos] = {key, V()};
        is_occupied[pos] = true;
        return store[pos].second;
    }

    bool contains(const K& key) const {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) return true;
            next_pos(pos);
        }
        return false;
    }

    void insert(const K& key, const V& val) {
        int pos = hsh(key);
        while (is_occupied[pos]) {
            if (store[pos].first == key) {
                store[pos].second = val;
                return;
            }
            next_pos(pos);
        }
        ++size_;
        store[pos] = {key, val};
        is_occupied[pos] = true;
    }
};
