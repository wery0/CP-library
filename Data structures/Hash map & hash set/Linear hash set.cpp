template<typename K, template<typename> class Allocator = std::allocator>
class linear_unordered_set {

    vector<K, Allocator<K>> store;

public:
    using iterator = vector<K, Allocator<K>>::iterator;
    linear_unordered_set() = default;
    template<typename I> linear_unordered_set(I first, I last): store(first, last) {}
    void reserve(size_t n) {store.reserve(n);}
    size_t size() const {return store.size();}
    size_t empty() const {return store.empty();}
    void clear() {store.clear();}
    void shrink_to_fit() {}
    size_t count(K key) const {return find(store.begin(), store.end(), key) != store.end();}
    K& operator[](K key) {for (auto& k : store) if (k == key) return k; store.emplace_back(key); return store.back();}
    void insert(K key) {for (auto& k : store) if (k == key) return; store.emplace_back(key);}
    void erase(K key) {for (size_t i = 0; i < store.size(); ++i) {if (store[i] == key) {store[i] = store.back();store.pop_back();break;}}}
    auto begin() {return store.begin();}
    auto begin() const {return store.begin();}
    auto end() {return store.end();}
    auto end() const {return store.end();}
};
