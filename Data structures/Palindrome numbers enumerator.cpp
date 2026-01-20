//Palindrome numbers are 0, 1, ..., 9, 11, 22, ..., 99, 101, ...
template<typename T>
class palindrome_numbers_enumerator {
    static_assert(is_integral_v<T>);
    static_assert(is_signed_v<T>);

    T cur = 0, num = 0, st = 1, h = 0, df = 1;
    int is_odd = 1;

    static T construct(T val, int is_odd) {
        T x = val;
        if (is_odd) x /= 10;
        for (; x; x /= 10) val = val * 10 + x % 10;
        return val;
    }

    static bool is_palindrome(const string& s) {
        return equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
    }

    static T str_to_num(const string& s) {
        return stoll(s);
    }

public:
    palindrome_numbers_enumerator(T x = 0) {set_cur(x);}

    void set_prev() {
        assert(cur > 0);
        --h, --num;
        if (h < st && cur != 1) {
            is_odd ? (df += df / 10, st /= 10) : (df /= 11, h = h * 10 + 9);
            is_odd ^= 1;
            cur -= 2;
        } else {
            int c0 = 0;
            for (T u = h + 1; u % 10 == 0; u /= 10) ++c0;
            T dd = df;
            if (is_odd && c0) --c0, dd += df / 10;
            while (c0--) dd /= 10;
            cur -= dd;
        }
    }

    void set_cur(T x) {
        string s = to_string(x);
        const size_t n = s.size();
        assert(is_palindrome(s));
        cur = x, is_odd = n & 1, st = 1;
        for (size_t i = 0; i < (n - 1) / 2; ++i) st *= 10;
        h = str_to_num(s.substr(0, (n + 1) / 2));
        df = is_odd ? st : st * 11;
        num = h + (is_odd ? st : st * 10) - 1;
    }

    void set_next() {
        ++h, ++num;
        if (h >= st * 10) {
            is_odd ? (df *= 11, h /= 10) : (df -= df / 11, st *= 10);
            is_odd ^= 1;
            cur += 2;
        } else {
            int c0 = 0;
            for (T u = h; u % 10 == 0; u /= 10) ++c0;
            T dd = df;
            if (is_odd && c0) --c0, dd += df / 10;
            while (c0--) dd /= 10;
            cur += dd;
        }
    }

    T get_cur() const {return cur;}

    //O(log(k))
    static T calc_kth_palindrome_number(T k) {
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

    //O(log(x))
    static T calc_lower_bound_palindrome(T x) {
        assert(x >= 0);
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        const size_t n = s.size();
        string l = s.substr(0, (n + 1) / 2);
        T c = construct(str_to_num(l), n & 1);
        return c >= x ? c : construct(str_to_num(l) + 1, n & 1);
    }

    //O(log(x))
    static T calc_inverse_lower_bound_palindrome(T x) {
        assert(x >= 0);
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        const size_t n = s.size();
        if (s[0] == '1' && all_of(s.begin() + 1, s.end(), [](char c) {return c == '0';})) return x - 1;
        string l = s.substr(0, (n + 1) / 2);
        T c = construct(str_to_num(l), n & 1);
        return c <= x ? c : construct(str_to_num(l) - 1, n & 1);
    }

    //O(log(x))
    static T calc_upper_bound_palindrome(T x) {return get_lower_bound_palindrome(x + 1);}
    static T calc_inverse_upper_bound_palindrome(T x) {return get_inverse_lower_bound_palindrome(x - 1);}
};
