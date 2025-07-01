template<typename K, typename V, template<typename> class Allocator = std::allocator>
class linear_unordered_map {

    vector<pair<K, V>, Allocator<pair<K, V>>> store;

    struct Iterator {
        vector<pair<K, V>, Allocator<pair<K, V>>>::iterator i;
        Iterator() = default;
        Iterator(vector<pair<K, V>, Allocator<pair<K, V>>>::iterator i): i(i) {}
        Iterator& operator++() {++i; return *this;}
        pair<const K, V>& operator*() {return reinterpret_cast<pair<const K, V>&>(*i);}
        bool operator==(const Iterator& rhs) const {return i == rhs.i;}
    };

public:
    using iterator = Iterator;
    linear_unordered_map() = default;
    template<typename I> linear_unordered_map(I first, I last): store(first, last) {}
    void reserve(size_t n) {store.reserve(n);}
    size_t size() const {return store.size();}
    size_t empty() const {return store.empty();}
    void clear() {store.clear();}
    size_t count(K key) const {for (auto& [k, v] : store) if (k == key) return 1; return 0;}
    V& operator[](K key) {for (auto& [k, v] : store) if (k == key) return v; store.emplace_back(key, V()); return store.back().second;}
    void insert(K key, V value) {for (auto& [k, v] : store) if (k == key) {v = value; return;}store.emplace_back(key, value);}
    void erase(K key) {for (size_t i = 0; i < store.size(); ++i) {if (store[i].first == key) {store[i] = store.back(); store.pop_back(); break;}}}
    auto begin() {return Iterator(store.begin());}
    auto begin() const {return Iterator(store.begin());}
    auto end() {return Iterator(store.end());}
    auto end() const {return Iterator(store.end());}
};
