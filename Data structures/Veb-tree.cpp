//Supports all std::set operations in O(log_2(LOG))
//Memory usage: poly(2 ^ (LOG / 2))?
const uint32_t NO = UINT32_MAX;                           //This value will be returned in lower_bound functions, if no answer exists. Change if need.
template<typename T, T LOG, typename ENABLE = void>       //Can correctly work with numbers in range [0; 2 ^ LOG)
class veb_tree {
    static_assert(is_unsigned_v<T>);
    static constexpr T ONE = 1;

    veb_tree<T, LOG / 2>* m[ONE << ((LOG + 1) / 2)] = {0};
    veb_tree<T, (LOG + 1) / 2> aux;
    T mn = NO, mx = NO;

    static T high(T x) {return x >> (LOG >> 1);}
    static T low(T x) {return x & ((ONE << (LOG >> 1)) - 1);}
    static T merge(T x, T y) {return (x << (LOG >> 1)) | y;}

public:
    veb_tree() = default;

    T get_min() const {return mn;}
    T get_max() const {return mx;}

    bool empty() const {return mx == NO;}

    bool count(T x) const {
        return x == mn || x == mx || m[high(x)] && m[high(x)]->count(low(x));
    }

    void insert(T x) {
        if (x == mn || x == mx) return;
        if (mx == NO) mn = mx = x;
        else if (mn == mx) mn = min(mn, x), mx = max(mx, x);
        else if (x < mn || x > mx) {
            T add = x < mn ? mn : mx;
            mn = min(mn, x);
            mx = max(mx, x);
            insert(add);
        } else if (LOG > 1) {
            T hi = high(x), lo = low(x);
            if (!m[hi]) m[hi] = new veb_tree<T, LOG / 2>();
            if (m[hi]->empty()) {
                aux.insert(hi);
            }
            m[hi]->insert(lo);
        }
    }

    void erase(T x) {
        if (mn == mx) {
            if (x == mn) mn = mx = NO;
            return;
        }
        if (x == mn) {
            if (aux.empty()) {mn = mx; return;}
            else mn = x = merge(aux.get_min(), m[aux.get_min()]->get_min());
        } else if (x == mx) {
            if (aux.empty()) {mx = mn; return;}
            else mx = x = merge(aux.get_max(), m[aux.get_max()]->get_max());
        }
        T hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty()) {
            m[hi]->erase(lo);
            if (m[hi]->empty()) aux.erase(hi);
        }
    }

    T upper_bound(T x) const {
        if (empty() || x >= mx) return NO;
        if (x < mn) return mn;
        if (LOG == 1) return mx;
        T hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->get_max() > lo) return merge(hi, m[hi]->upper_bound(lo));
        T next_hi = aux.upper_bound(hi);
        return next_hi == NO ? mx : merge(next_hi, m[next_hi]->get_min());
    }

    T lower_bound(T x) const {
        if (empty() || x > mx) return NO;
        if (x <= mn) return mn;
        if (LOG == 1) return mx;
        T hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->get_max() >= lo) return merge(hi, m[hi]->lower_bound(lo));
        T next_hi = aux.upper_bound(hi);
        return next_hi == NO ? mx : merge(next_hi, m[next_hi]->get_min());
    }

    T inverse_upper_bound(T x) const {
        if (empty() || x <= mn) return NO;
        if (x > mx) return mx;
        if (LOG == 1) return mn;
        T hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->get_min() < lo) return merge(hi, m[hi]->inverse_upper_bound(lo));
        T prev_hi = aux.inverse_upper_bound(hi);
        return prev_hi == NO ? mn : merge(prev_hi, m[prev_hi]->get_max());
    }

    T inverse_lower_bound(T x) const {
        if (empty() || x < mn) return NO;
        if (x >= mx) return mx;
        if (LOG == 1) return mn;
        T hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->get_min() <= lo) return merge(hi, m[hi]->inverse_lower_bound(lo));
        T prev_hi = aux.inverse_upper_bound(hi);
        return prev_hi == NO ? mn : merge(prev_hi, m[prev_hi]->get_max());
    }
};

template<typename T, T LOG>
class veb_tree<T, LOG, enable_if_t<LOG <= 6>> {
    static constexpr uint64_t ONE = 1;

    uint64_t mask = 0;

public:
    veb_tree() = default;

    T get_min() const {return __builtin_ctzll(mask);}
    T get_max() const {return __lg(mask);}
    bool empty() const {return mask == 0;}
    int count(T x) const {return mask >> x & 1;}
    void insert(T x) {mask |= ONE << x;}
    void erase(T x) {mask &= ~(ONE << x);}

    T upper_bound(T x) const {
        uint64_t rem = x == 63 ? 0 : mask >> (x + 1);
        return rem ? x + 1 + __builtin_ctzll(rem) : NO;
    }

    T lower_bound(T x) const {
        uint64_t rem = mask >> x;
        return rem ? x + __builtin_ctzll(rem) : NO;
    }

    T inverse_upper_bound(T x) const {
        uint64_t rem = mask & ((ONE << x) - 1);
        return rem ? __lg(rem) : NO;
    }

    T inverse_lower_bound(T x) const {
        uint64_t rem = x == 63 ? mask : mask & ((ONE << (x + 1)) - 1);
        return rem ? __lg(rem) : NO;
    }
};
