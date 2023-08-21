template<typename T>
class fully_persistent_array {
    struct Node {
        size_t l = 0;
        size_t r = 0;
        T val;

        Node(T val): val(val) {}
        Node(size_t l, size_t r, T val): l(l), r(r), val(val) {}
        Node(Node* n): l(n->l), r(n->r), val(n->val) {}
    };
    vector<Node> store{Node(0)};
    vector<size_t> roots;
    size_t n;

    template<typename I>
    size_t build(I first, I last) {
        size_t n = last - first;
        if (n == 0) return 0;
        if (n == 1) {
            store.emplace_back(*first);
            return store.size() - 1;
        }
        size_t mid = n / 2;
        store.emplace_back(build(first, first + mid), build(first + mid + 1, last), *(first + mid));
        return store.size() - 1;
    }

    size_t build_const(size_t n, T val) {
        if (n == 0) return 0;
        if (n == 1) {
            store.emplace_back(val);
            return store.size() - 1;
        }
        store.emplace_back(build_const(n / 2, val), build_const(n - n / 2 - 1, val), val);
        return store.size() - 1;
    }

public:
    fully_persistent_array() = default;
    fully_persistent_array(size_t n): n(n) {
        store.reserve(n + 5);
        roots = {build_const(n, T{})};
    }
    fully_persistent_array(size_t n, T val): n(n) {
        store.reserve(n + 5);
        roots = {build_const(n, val)};
    }
    template<typename I>
    fully_persistent_array(I first, I last, typename enable_if<!is_integral_v<I>>::type* = 0): n(last - first) {
        store.reserve(n + 5);
        roots = {build(first, last)};
    }
    template<typename U>
    fully_persistent_array(const initializer_list<U>& l): n(l.size()) {
        store.reserve(n + 5);
        roots = {build(l.begin(), l.end())};
    }

    size_t size() const {return n;}

    size_t get_cur_version() const {return roots.size() - 1;}

    //For q set operations use smth like sz = n + q * __lg(n) / 2
    void reserve(size_t sz) {store.reserve(sz);}

    //<O(log(n)), O(log(n))>
    //Creates new version
    void set_value(size_t version, size_t pos, T val) {
        assert(version < roots.size());
        assert(pos < n);
        store.emplace_back(store[roots[version]]);
        size_t root = store.size() - 1;
        roots.push_back(root);
        size_t cur = root;
        for (size_t sz = n;;) {
            size_t szl = sz / 2;
            if (pos == szl) {store[cur].val = val; break;}
            if (pos < szl) {
                store.emplace_back(store[store[cur].l]);
                store[cur].l = store.size() - 1;
                cur = store.size() - 1;
                sz = szl;
            } else {
                store.emplace_back(store[store[cur].r]);
                store[cur].r = store.size() - 1;
                cur = store.size() - 1;
                sz -= szl + 1;
                pos -= szl + 1;
            }
        }
    }

    //O(log(n))
    T operator()(size_t version, size_t pos) const {
        assert(version < roots.size());
        assert(pos < n);
        size_t cur = roots[version];
        for (size_t sz = n;;) {
            size_t szl = sz / 2;
            if (pos == szl) return store[cur].val;
            if (pos < szl) cur = store[cur].l, sz = szl;
            else cur = store[cur].r, sz -= szl + 1, pos -= szl + 1;
        }
    }

    //O(nlog(n))
    vector<T> get_version_as_vector(size_t version) const {
        assert(version < roots.size());
        vector<T> ans(n);
        for (size_t i = 0; i < n; ++i) {
            ans[i] = operator()(version, i);
        }
        return ans;
    }
};
