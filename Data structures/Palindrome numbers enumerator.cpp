//Palindrome numbers are 0, 1, ..., 9, 11, 22, ..., 99, 101, ...
template<typename T>
class palindrome_numbers_enumerator {
    static_assert(is_integral_v<T>);
    static_assert(is_signed_v<T>);

    T st = 1, q1 = 0, q2 = 1;
    T cur = 0, num = 0;
    int is_odd = 1;

    T construct(T val, int is_odd) const {
        T x = val;
        if (is_odd) x /= 10;
        for (; x; x /= 10) val = val * 10 + x % 10;
        return val;
    }

    bool is_palindrome(const string& s) const {
        return equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
    }

public:
    palindrome_numbers_enumerator(T x = 0) {set_cur(x);}

    void set_cur(T x) {
        string s = to_string(x);
        const size_t n = s.size();
        assert(is_palindrome(s));
        cur = x, is_odd = n & 1, st = 1;
        for (size_t i = 0; i < (n - 1) / 2; ++i) st *= 10;
        if (is_odd) {
            q1 = stoll(s.substr(0, (n + 1) / 2));
            q2 = st;
        } else {
            q1 = st * 10;
            q2 = stoll(s.substr(0, (n + 1) / 2));
        }
        num = q1 + q2 - is_odd;
    }

    void set_prev() {cur = get_prev();}
    void set_next() {cur = get_next();}

    T get_prev() {
        --(is_odd ? q1 : q2);
        --num;
        if ((is_odd ? q1 : q2) < st && q1 != 0) {
            if (!is_odd) ++q2, --q1;
            else ++q1, --q2;
            if (is_odd) st /= 10;
            is_odd ^= 1;
        }
        return construct(is_odd ? q1 : q2, is_odd);
    }

    T get_next() {
        ++(is_odd ? q1 : q2);
        ++num;
        if ((is_odd ? q1 : q2) >= st * 10) {
            is_odd ^= 1;
            if (q2 != st) st = st ? st * 10 : 1;
        }
        return construct(is_odd ? q1 : q2, is_odd);
    }

    T get_kth_palindrome_number(T k) const {
        string s = to_string(k);
        T pw10 = 1;
        while (pw10 * 10 <= k) pw10 *= 10;
        if (k <= pw10 + pw10 / 10 - 2) {
            k -= pw10 / 10; ++k;
            return construct(k, 1);
        }
        if (k <= pw10 * 2 - 2) {
            k -= pw10; ++k;
            return construct(k, 0);
        }
        return construct(k - pw10 + 1, 1);
    }

    T get_lower_bound_palindrome(T x) const {
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        const size_t n = s.size();
        string l = s.substr(0, (n + 1) / 2);
        T c = construct(stoll(l), n & 1);
        return c >= x ? c : construct(stoll(l) + 1, n & 1);
    }

    T get_inverse_lower_bound_palindrome(T x) const {
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        const size_t n = s.size();
        if (count(s.begin(), s.end(), '0') == n - 1) return x - 1;
        string l = s.substr(0, (n + 1) / 2);
        T c = construct(stoll(l), n & 1);
        return c <= x ? c : construct(stoll(l) - 1, n & 1);
    }

    T get_upper_bound_palindrome(T x) const {return get_lower_bound_palindrome(x + 1);}
    T get_inverse_upper_bound_palindrome(T x) const {return get_inverse_lower_bound_palindrome(x - 1);}
    T get_cur() const {return cur;}
};