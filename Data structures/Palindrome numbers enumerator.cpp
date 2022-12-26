class palindrome_numbers_enumerator {

    ll st = 1, is_odd = 1, q1 = 0, q2 = 1;
    ll cur = 0, num = 0;

    ll construct(ll val, ll is_odd) {
        ll x = val;
        if (is_odd) x /= 10;
        for (; x; x /= 10) val = val * 10 + x % 10;
        return val;
    }

    bool is_palindrome(string& s) {
        int a = s.size();
        for (int q = 0; q < a - q - 1; ++q) {
            if (s[q] != s[a - q - 1]) return 0;
        }
        return 1;
    }

public:
    void set_cur(ll x) {
        string s = to_string(x);
        int a = s.size();
        assert(is_palindrome(s));
        cur = x;
        is_odd = a & 1;
        st = 1; for (int q = 0; q < (a - 1) / 2; ++q) st *= 10;
        if (is_odd) {
            q1 = stoll(s.substr(0, (a + 1) / 2));
            q2 = st;
        } else {
            q1 = st * 10;
            q2 = stoll(s.substr(0, (a + 1) / 2));
        }
        num = q1 + q2 - is_odd;
    }

    ll get_prev() {
        --(is_odd ? q1 : q2);
        --num;
        if ((is_odd ? q1 : q2) < st && q1 != 0) {
            if (!is_odd) {
                ++q2;
                --q1;
            } else {
                ++q1;
                --q2;
            }
            if (is_odd) st /= 10;
            is_odd ^= 1;
        }
        ll res = construct(is_odd ? q1 : q2, is_odd);
        return res;
    }

    ll get_next() {
        ++(is_odd ? q1 : q2);
        ++num;
        if ((is_odd ? q1 : q2) >= st * 10) {
            is_odd ^= 1;
            if (q2 != st) st = st ? st * 10 : 1;
        }
        ll res = construct(is_odd ? q1 : q2, is_odd);
        return res;
    }

    void set_prev() {cur = get_prev();}
    void set_next() {cur = get_next();}

    ll get_kth_palindrome_number(ll k) {
        string s = to_string(k);
        ll pw10 = 1;
        for (; pw10 * 10 <= k;) pw10 *= 10;
        if (k <= pw10 + pw10 / 10 - 2) {
            k -= pw10 / 10; ++k;
            return construct(k, 1);
        }
        if (k <= pw10 * 2 - 2) {
            k -= pw10; ++k;
            return construct(k, 0);
        }
        k -= pw10; ++k;
        return construct(k, 1);
    }

    ll get_lower_bound_palindrome(ll x) {
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        int a = s.size();
        string l = s.substr(0, (a + 1) / 2);
        ll c = construct(stoll(l), a & 1);
        return c >= x ? c : construct(stoll(l) + 1, a & 1);
    }

    ll get_inverse_lower_bound_palindrome(ll x) {
        string s = to_string(x);
        if (is_palindrome(s)) return x;
        int a = s.size();
        if (count(all(s), '0') == a - 1) return x - 1;
        string l = s.substr(0, (a + 1) / 2);
        ll c = construct(stoll(l), a & 1);
        return c <= x ? c : construct(stoll(l) - 1, a & 1);
    }

    ll get_upper_bound_palindrome(ll x) {
        return get_lower_bound_palindrome(x + 1);
    }

    ll get_inverse_upper_bound_palindrome(ll x) {
        return get_inverse_lower_bound_palindrome(x - 1);
    }

    ll get_cur() {
        return cur;
    }
};
//0, 1, ..., 9, 11, 22, ..., 99, 101, ...