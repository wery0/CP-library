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
        for (int i = 0; first != last; ++first, ++i) {
            store[i][0].second = *first;
        }
    }

    size_t size() const {return n;}

    int get_cur_version() const {return cur_version;}

    //<O(1), O(1)>
    //Creates new version
    void set_value(int pos, T val) {
        assert(0 <= pos && pos < n);
        store[pos].emplace_back(++cur_version, val);
    }

    //O(log(n))
    T operator()(int version, int pos) const {
        assert(0 <= version && version <= cur_version);
        assert(0 <= pos && pos < n);
        auto it = upper_bound(all(store[pos]), version, [&](int v, const pair<int, T>& r) {return v < r.first;});
        return (*prev(it)).second;
    }

    //O(nlog(n))
    vector<T> get_version_as_vector(int version) const {
        assert(0 <= version && version <= cur_version);
        vector<T> ans(n);
        for (int q = 0; q < n; ++q) {
            ans[q] = operator()(version, q);
        }
        return ans;
    }
};