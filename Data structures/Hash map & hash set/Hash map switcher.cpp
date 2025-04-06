template<typename K, typename V, template<typename> class Allocator = std::allocator>
class unordered_map_switcher {

    enum Emode {LINEAR, CONST};
    using linear_cont = linear_unordered_map<K, V, Allocator>;
    using const_cont = unordered_map<K, V, hash<K>, equal_to<K>, Allocator<pair<const K, V>>>;

    Emode mode = LINEAR;
    linear_cont lus;
    const_cont cus;

    void change_mode() {
        if (mode == LINEAR && lus.size() >= 8) {
            mode = CONST;
            cus = const_cont(lus.begin(), lus.end());
            lus = linear_cont();
        } else if (mode == CONST && cus.size() <= 4) {
            mode = LINEAR;
            lus = linear_cont(cus.begin(), cus.end());
            cus = const_cont();
        }
    }

    struct Iterator {
        linear_cont::iterator i1;
        const_cont::iterator i2;
        Emode mode;
        Iterator() = default;
        Iterator(linear_cont::iterator i1, const_cont::iterator i2, Emode mode): i1(i1), i2(i2), mode(mode) {}
        Iterator& operator++() {mode == LINEAR ? void(++i1) : void(++i2); return *this;}
        pair<const K, V>& operator*() {return mode == LINEAR ? *i1 : *i2;}
        bool operator==(const Iterator& rhs) const {return i1 == rhs.i1 && i2 == rhs.i2;}
    };

public:
    unordered_map_switcher() = default;

    void reserve(size_t n) {mode == LINEAR ? lus.reserve(n) : cus.reserve(n);}
    size_t size() const {return mode == LINEAR ? lus.size() : cus.size();}
    bool empty() const {return mode == LINEAR ? lus.empty() : cus.empty();}
    size_t count(K key) const {return mode == LINEAR ? lus.count(key) : cus.count(key);}
    V& operator[](K key) {return mode == LINEAR ? lus[key] : cus[key];}
    void insert(K key) {mode == LINEAR ? void(lus.insert(key)) : void(cus.insert(key)); change_mode();}
    void erase(K key) {mode == LINEAR ? void(lus.erase(key)) : void(cus.erase(key)); change_mode();}
    void clear() {mode == LINEAR ? lus.clear() : cus.clear();}

    auto begin() {return Iterator(lus.begin(), cus.begin(), mode);}
    auto begin() const {return Iterator(lus.begin(), cus.begin(), mode);}
    auto end() {return Iterator(lus.end(), cus.end(), mode);}
    auto end() const {return Iterator(lus.end(), cus.end(), mode);}
};
