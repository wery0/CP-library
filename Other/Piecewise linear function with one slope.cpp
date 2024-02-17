/*
Represents f(x), s. t. f(x) is linear on range [l_x, r_x] with tangent coefficient 1 and constant elsewhere
Aka piecewise linear function which can be defined like that:
f(x) = l_val, x <= l_x
f(x) = l_val + (x - l_x), l_x <= x <= r_x
f(x) = l_val + (r_x - l_x), r_x <= x
*/
template<typename T>
struct plf1 {
    static constexpr T NO = numeric_limits<ll>::max();
    T l_val;
    T l_x, r_x;

    plf1() = default;

    plf1(T l_val, T l_x, T r_x): l_val(l_val), l_x(l_x), r_x(r_x) {
        assert(l_x <= r_x);
    }

    T get_min_val() const {
        return l_val;
    }

    T get_max_val() const {
        return l_val + r_x - l_x;
    }

    //Returns min x, s. t. f(x) >= y. If that min == -inf, returns l_x. If it doesn't exist, returns NO
    T when_geq(T y) const {
        if (y <= get_min_val()) return l_x;
        if (y > get_max_val()) return NO;
        return l_x + y - l_val;
    }

    T operator()(T x) const {
        return l_val + min(max(x, l_x), r_x) - l_x;
    }

    //(*this)(f)(x) == (*this)(f(x))
    plf1<T> operator()(const plf1<T>& f) {
        T l_val = (*this)(f.l_val);
        T when = f.when_geq(l_x);
        T l_x = when != plf1<T>::NO ? when : 0;
        T r_x = l_x + ((*this)(f.get_max_val()) - l_val);
        return plf1<T>(l_val, l_x, r_x);
    }
};
