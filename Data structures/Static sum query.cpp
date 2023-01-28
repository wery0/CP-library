template<typename T>
struct static_sum_query {
    vector<T> m;

    static_sum_query() = default;
    template<typename I>
    static_sum_query(I first, I last): m(last - first + 1) {
        for (auto it = m.begin() + 1; first != last; ++first, ++it) {
            *it = *(it - 1) + *first;
        }
    }
    template<typename T_arr> static_sum_query(T_arr& m) {
        (*this) = static_sum_query(m.begin(), m.end());
    }

    inline T query(const int l, const int r) const {
        return m[r + 1] - m[l];
    }
};