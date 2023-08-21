//All operations are <O(1), O(1)>
template<typename T>
class fully_persistent_stack {
    vector<size_t> pr{0};
    vector<T> val{T{}};
    vector<size_t> sz{0};

public:
    fully_persistent_stack() = default;

    void reserve(size_t n) {
        pr.reserve(n);
        val.reserve(n);
        sz.reserve(n);
    }

    size_t size(size_t version) const {
        assert(version < pr.size());
        return sz[version];
    }

    //Creates new version
    void push(size_t version, T x) {
        assert(version < pr.size());
        pr.push_back(version);
        val.push_back(x);
        sz.push_back(sz[version] + 1);
    }

    //Creates new version
    T pop(size_t version) {
        assert(version < pr.size());
        assert(size(version));
        pr.push_back(pr[version]);
        val.push_back(val[pr[version]]);
        sz.push_back(sz[pr[version]]);
        return val[version];
    }

    T top(size_t version) const {
        assert(version < pr.size());
        return val[version];
    }
};
