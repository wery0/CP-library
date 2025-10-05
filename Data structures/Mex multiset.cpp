template<typename T>
class mex_multiset {
    vector<int> mn;
    T N = 0, U = 0;
    size_t sm = 0;

    void point_add(int pos, int val) {
        pos += U;
        mn[pos] += val;
        sm += val;
        assert(mn[pos] >= 0);
        for (pos >>= 1; pos; pos >>= 1) mn[pos] = min(mn[pos << 1], mn[pos << 1 | 1]);
    }

public:
    mex_multiset() = default;
    mex_multiset(T N): N(N), U(N & (N - 1) ? 2 << __lg(N) : N) {
        assert(N >= 0);
        mn.resize(U * 2);
    }

    size_t size() const {
        return sm;
    }

    void clear() {
        sm = 0;
        fill(mn.begin(), mn.end(), 0);
    }

    void insert(T x) {
        assert(0 <= x && x <= N);
        point_add(x, 1);
    }

    void erase_one(T x) {
        assert(0 <= x && x <= N);
        point_add(x, -1);
    }

    void erase_all(T x) {
        assert(0 <= x && x <= N);
        point_add(x, -mn[U + x]);
    }

    bool contains(T x) const {
        assert(0 <= x && x <= N);
        return mn[U + x] > 0;
    }

    int count(T x) {
        assert(0 <= x && x <= N);
        return mn[U + x];
    }

    T get_mex() const {
        if (mn[1] != 0) return N + 1;
        size_t v = 1;
        for (; v < U; ) {
            v <<= 1;
            if (mn[v]) v |= 1;
        }
        return v - U;
    }
};
