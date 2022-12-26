template<typename T>
struct offline_seg_add {

    int n;
    vector<T> store;

    offline_seg_add() = default;
    offline_seg_add(int n): n(n), store(n + 1) {}
    template<typename I>
    offline_seg_add(I first, I last): n(last - first), store(n + 1) {
        for (int q = 0; q < n; ++q) {
            store[q] += *(first + q);
            store[q + 1] -= *(first + q);
        }
    }
    template<typename T_arr>
    offline_seg_add(T_arr& m) {
        (*this) = offline_seg_add(all(m));
    }

    void seg_add(int l, int r, T val) {
        store[l] += val;
        store[r + 1] -= val;
    }

    void clear() {
        fill(store.begin(), store.end(), 0);
    }

    void get_result(vector<T>& m) {
        assert(n <= m.size());
        m[0] = store[0];
        for (int q = 1; q < n; ++q) {
            m[q] = m[q - 1] + store[q];
        }
    }

    vector<T> get_result() {
        vector<T> m(n);
        get_result(m);
        return m;
    }
};