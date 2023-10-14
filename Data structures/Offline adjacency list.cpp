template<typename T>
class offline_adjacency_list {
    size_t n;
    vector<T> store;
    vector<int> fir;
    vector<pair<int, T>> acc;
    bool is_prepared = 0;

    struct row_range {
        typename vector<T>::iterator first, last;
        auto begin() const {return first;}
        auto end() const {return last;}

        int size() const {return last - first;}
        bool empty() const {return first == last;}
        T& operator[](int i) {return *(first + i);}
        const T& operator[](int i) const {return *(first + i);}
    };

public:
    offline_adjacency_list() = default;

    offline_adjacency_list(size_t n, size_t entries = 0): n(n), fir(n + 2) {
        acc.reserve(entries);
    }

    void clear() {
        store.clear();
        fill(fir.begin(), fir.end(), 0);
        is_prepared = false;
    }

    void add_entry(size_t i, const T& val) {
        assert(i < n && !is_prepared);
        acc.emplace_back(i, val);
    }

    void prepare() {
        store.resize(acc.size());
        for (const auto& [i, val] : acc) ++fir[i + 2];
        partial_sum(fir.begin(), fir.end(), fir.begin());
        for (const auto& [i, val] : acc) store[fir[i + 1]++] = val;
        fir.pop_back();
        acc.clear();
        acc.shrink_to_fit();
        is_prepared = true;
    }

    row_range operator[](int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    const row_range operator[](int v) const {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
};
//Usage: offline_adjacency_list<T> l(N, [entries]), where T is the type of info you want to store.
//Before using it, add all entries and then call prepare().
//Usually much more efficient than vector<vector<T>>.
