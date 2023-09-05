template<typename T>
class merge_sort_tree {

    static constexpr T inf = numeric_limits<T>::max();

    size_t n, U;
    vector<vector<T>> store;

    int work_seg(vector<T>& arr, T val_l, T val_r) {
        auto it_l = lower_bound(arr.begin(), arr.end(), val_l);
        auto it_r = upper_bound(arr.begin(), arr.end(), val_r);
        return it_r - it_l;
    }

    int work_leq(vector<T>& arr, T val_r) {
        auto it_r = upper_bound(arr.begin(), arr.end(), val_r);
        return it_r - arr.begin();
    }

    T work_max_leq(vector<T>& arr, T x, T NO_ANSWER) {
        auto it_r = upper_bound(arr.begin(), arr.end(), x);
        return it_r == arr.begin() ? NO_ANSWER : *--it_r;
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

    //Returns # values in range [val_l, val_r] on segment [l, r]
    //O(log(n)^2)
    int seg_count_seg(size_t l, size_t r, T val_l, T val_r) {
        l += U, r += U;
        int ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_seg(store[l], val_l, val_r);
            if (~r & 1) ans += work_seg(store[r], val_l, val_r);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    //Returns # values <= val_r on segment [l, r]
    //O(log(n)^2)
    int seg_count_leq(size_t l, size_t r, T val_r) {
        l += U, r += U;
        int ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_leq(store[l], val_r);
            if (~r & 1) ans += work_leq(store[r], val_r);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    //Returns maximum number on segment [l, r], which <= x
    //If all numbers on segment [l, r] > x, then returns:
    //  NO_ANSWER, if it`s set
    //  x + 1      otherwise
    //O(log(n)^2)
    int seg_max_leq(size_t l, size_t r, T x, T NO_ANSWER = -inf) {
        l += U, r += U;
        T ans = -inf;
        while (l <= r) {
            if (l & 1) ans = max(ans, work_max_leq(store[l], x, -inf));
            if (~r & 1) ans = max(ans, work_max_leq(store[r], x, -inf));
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans != -inf ? ans : NO_ANSWER == -inf ? x + 1 : NO_ANSWER;
    }
};
