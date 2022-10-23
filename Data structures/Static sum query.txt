template<typename T>
struct static_sum_query {
    vec<T> m;

    static_sum_query() = default;

    template<typename I>
    static_sum_query(I f, I l) {
        m.resize(l - f + 1);
        for (auto it = m.begin() + 1; f != l; ++f, ++it) {
            *it = *(it - 1) + *f;
        }
    }

    template<typename T_arr> static_sum_query(T_arr& m) {
        (*this) = static_sum_query(all(m));
    }

    inline T query(const int l, const int r) const {
        return m[r + 1] - m[l];
    }
};