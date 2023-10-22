template<typename T>
class offline_seg_add {

    size_t n;
    vector<T> store;

public:
    offline_seg_add() = default;
    offline_seg_add(size_t n): n(n), store(n + 1) {}
    template<typename I>
    offline_seg_add(I first, I last): n(last - first), store(n + 1) {
        for (size_t i = 0; i < n; ++i) {
            store[i] += *(first + i);
            store[i + 1] -= *(first + i);
        }
    }
    template<typename T_arr>
    offline_seg_add(T_arr& arr, typename enable_if<!is_integral_v<T_arr>>::type* = 0) {
        (*this) = offline_seg_add(arr.begin(), arr.end());
    }

    void reset() {
        fill(store.begin(), store.end(), 0);
    }

    void seg_add(size_t l, size_t r, T val) {
        store[l] += val;
        store[r + 1] -= val;
    }

    vector<T> get_result() const {
        vector<T> ans(n);
        partial_sum(store.begin(), store.begin() + n, ans.begin());
        return ans;
    }
};
