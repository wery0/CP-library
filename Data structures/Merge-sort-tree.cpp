template<typename T>
class merge_sort_tree {
    int G, U;
    vector<vector<T>> store;

    inline int work(vector<T>& arr, T val_l, T val_r) {
        auto it_r = upper_bound(all(arr), val_r);
        auto it_l = lower_bound(all(arr), val_l);
        return it_r - it_l;
    }

public:
    template<typename Iterator>
    merge_sort_tree(Iterator first, Iterator last): U(last - first), G(U * 2), store(G) {
        for (auto it = store.begin() + U; first != last; ++first, ++it) {
            *it = {*first};
        }
        for (int q = U; --q; ) {
            merge(all(store[q << 1]), all(store[q << 1 | 1]), back_inserter(store[q]));
        }
    }

    template<typename T_arr>
    merge_sort_tree(T_arr& m) {
        (*this) = merge_sort_tree(all(m));
    }

    //Counts # values in range [val_l, val_r] on segment [l, r]
    //O(log(n)^2)
    int query(int l, int r, T val_l, T val_r) {
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