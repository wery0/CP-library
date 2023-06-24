template<typename T>
class merge_sort_tree {
    size_t n, U;
    vector<vector<T>> store;

    int work(vector<T>& arr, T val_l, T val_r) {
        auto it_l = lower_bound(arr.begin(), arr.end(), val_l);
        auto it_r = upper_bound(arr.begin(), arr.end(), val_r);
        return it_r - it_l;
    }

public:
    template<typename Iterator>
    merge_sort_tree(Iterator first, Iterator last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n), store(U * 2) {
        if (n == 0) return;
        for (auto it = store.begin() + U; first != last; ++first, ++it) {
            *it = {*first};
        }
        for (size_t i = U; --i; ) {
            merge(store[i << 1].begin(), store[i << 1].end(), store[i << 1 | 1].begin(), store[i << 1 | 1].end(), back_inserter(store[i]));
        }
    }

    template<typename T_arr>
    merge_sort_tree(T_arr& m) {
        (*this) = merge_sort_tree(m.begin(), m.end());
    }

    //Counts # values in range [val_l, val_r] on segment [l, r]
    //O(log(n)^2)
    int query(size_t l, size_t r, T val_l, T val_r) {
        l += U, r += U;
        int ans = 0;
        while (l <= r) {
            if (l & 1) ans += work(store[l], val_l, val_r);
            if (~r & 1) ans += work(store[r], val_l, val_r);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }
};