template<typename T>
class partially_persistent_array {
    vector<vector<pair<int, T>>> store;
    int cur_version = 0;

public:
    partially_persistent_array(int n): store(n, {{0, T{}}}) {}
    template<typename I>
    partially_persistent_array(I first, I last) {
        int n = last - first;
        store.resize(n, {{0, T{}}});
        for (int i = 0; first != last; ++first, ++i) {
            store[i][0].second = *first;
        }
    }
    template<typename U>
    partially_persistent_array(const initializer_list<U>& l) {
        *this = partially_persistent_array(all(l));
    }

    int get_cur_version() const {return cur_version;}

    //O(1)
    void set_value(int pos, T val) {
        assert(0 <= pos && pos < store.size());
        store[pos].emplace_back(++cur_version, val);
    }

    //O(log(n))
    T get_value(int version, int pos) {
        assert(0 <= pos && pos < store.size());
        assert(0 <= version && version <= cur_version);
        auto it = upper_bound(all(store[pos]), version, [&](int v, const pair<int, T>& r) {return v < r.first;});
        assert(it != store[pos].begin());
        return (*prev(it)).second;
    }

    //O(nlog(n))
    vector<T> get_version_as_vector(int version) {
        assert(0 <= version && version <= cur_version);
        vector<T> ans(store.size());
        for (int q = 0; q < store.size(); ++q) {
            ans[q] = get_value(version, q);
        }
        return ans;
    }
};