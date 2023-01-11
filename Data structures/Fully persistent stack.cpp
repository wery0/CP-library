//All operations are <O(1), O(1)>
template<typename T>
class fully_persistent_stack {
    vector<int> pr{ -1};
    vector<T> val{T{}};
    vector<int> sz{0};

public:
    fully_persistent_stack() = default;

    void reserve(size_t n) {
        pr.reserve(n);
        val.reserve(n);
        sz.reserve(n);
    }

    size_t size(int version) const {
        assert(0 <= version && version < pr.size());
        return sz[version];
    }

    //Creates new version
    void push(int version, T x) {
        assert(0 <= version && version < pr.size());
        pr.push_back(version);
        val.push_back(x);
        sz.push_back(sz[version] + 1);
    }

    //Creates new version
    T pop(int version) {
        assert(0 <= version && version < pr.size());
        assert(pr[version] != -1);
        pr.push_back(pr[version]);
        val.push_back(val[pr[version]]);
        sz.push_back(sz[pr[version]]);
        return val[version];
    }

    T top(int version) const {
        assert(0 <= version && version < pr.size());
        return val[version];
    }
};