template<typename T>
class partially_persistent_array {
    vector<vector<pair<int, T>>> store;
    int cur_version = 0;
    size_t n = 0;

public:
    partially_persistent_array() = default;
    partially_persistent_array(size_t n): n(n), store(n, {{0, T{}}}) {}
    partially_persistent_array(size_t n, T val): n(n), store(n, {{0, val}}) {}
    template<typename U>
    partially_persistent_array(const initializer_list<U>& l) {
        *this = partially_persistent_array(l.begin(), l.end());
    }
    template<typename I>
    partially_persistent_array(I first, I last, typename enable_if<!is_integral_v<I>>::type* = 0) {
        n = last - first;
        store.resize(n, {{0, T{}}});
        for (size_t i = 0; first != last; ++first, ++i) {
            store[i][0].second = *first;
        }
    }

    size_t size() const {return n;}

    int get_cur_version() const {return cur_version;}

    //<O(1), O(1)>
    //Creates new version
    void set_value(size_t pos, T val) {
        assert(pos < n);
        store[pos].emplace_back(++cur_version, val);
    }

    //O(log(n))
    T operator()(size_t version, size_t pos) const {
        assert(version <= cur_version);
        assert(pos < n);
        auto it = upper_bound(store[pos].begin(), store[pos].end(), version, [&](int v, const pair<int, T>& r) {return v < r.first;});
        return (*prev(it)).second;
    }

    //O(nlog(n))
    vector<T> get_version_as_vector(size_t version) const {
        assert(version <= cur_version);
        vector<T> ans(n);
        for (size_t i = 0; i < n; ++i) {
            ans[i] = operator()(version, i);
        }
        return ans;
    }
};
