template<typename T>
class kahan_fp_summator {
    static_assert(is_floating_point_v<T>);

    T sum = 0, err = 0;

public:
    kahan_fp_summator() = default;

    void clear() {
        sum = err = 0;
    }

    void add(T x) {
        volatile T tmp = sum + x;
        volatile T dif = tmp - sum;
        err += x - dif;
        sum = tmp;
    }

    T get_sum() {
        return sum + err;
    }
};
