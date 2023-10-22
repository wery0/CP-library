template<typename T>
class offline_seg_add_progression {

    size_t n;
    vector<T> store;

public:
    offline_seg_add_progression() = default;
    offline_seg_add_progression(size_t n): n(n), store(n + 2) {}
    template<typename I>
    offline_seg_add_progression(I first, I last): n(last - first), store(n + 2) {
        for (size_t i = 0; i < n; ++i) {
            store[i] += *(first + i);
            store[i + 1] -= (*(first + i)) * 2;
            store[i] += *(first + i);
        }
    }
    template<typename T_arr>
    offline_seg_add_progression(T_arr& arr, typename enable_if<!is_integral_v<T_arr>>::type* = 0) {
        (*this) = offline_seg_add(arr.begin(), arr.end());
    }

    void reset() {
        fill(store.begin(), store.end(), 0);
    }

    //arr[l + i] += a0 + b * i, 0 <= i < r - l + 1
    void seg_add_progression(size_t l, size_t r, T a0, T b) {
        store[l] += a0;
        store[l + 1] += b - a0;
        store[r + 1] -= b * (r - l + 1) + a0;
        store[r + 2] += b * (r - l) + a0;
    }

    vector<T> get_result() const {
        vector<T> ans(n);
        partial_sum(store.begin(), store.begin() + n, ans.begin());
        partial_sum(ans.begin(), ans.end(), ans.begin());
        return ans;
    }
};
