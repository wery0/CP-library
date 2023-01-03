const uint NO = UINT32_MAX;  //This value will be returned in lower_bound functions, if no answer exists. Change if need.
template<uint K>
class VebTree {
    VebTree<K / 2>* m[1 << ((K + 1) / 2)];
    VebTree<(K + 1) / 2>* aux;

    inline uint high(uint x) const {
        return x >> (K >> 1);
    }

    inline uint low(uint x) const {
        return x & ((1 << (K >> 1)) - 1);
    }

    inline uint merge(uint x, uint y) const {
        return (x << (K >> 1)) + y;
    }

public:
    uint mn, mx;

    VebTree() {
        memset(m, 0, sizeof(m));
        aux = 0;
        mn = mx = NO;
    }

    inline bool empty() const {
        return mx == NO;
    }

    bool count(uint x) {
        if (x == mn || x == mx) return 1;
        return m[high(x)] && m[high(x)]->count(low(x));
    }

    void insert(uint x) {
        if (mx == NO) mn = mx = x;
        else if (mn == mx) mn = min(mn, x), mx = max(mx, x);
        else if (x < mn || x > mx) {
            uint add = x < mn ? mn : mx;
            mn = min(mn, x);
            mx = max(mx, x);
            insert(add);
        } else if (K > 1) {
            uint hi = high(x), lo = low(x);
            if (!m[hi]) m[hi] = new VebTree<K / 2>();
            if (m[hi]->empty()) {
                if (!aux) aux = new VebTree<(K + 1) / 2>();
                aux->insert(hi);
            }
            m[hi]->insert(lo);
        }
    }

    void erase(uint x) {
        if (mn == mx) {
            mn = mx = NO;
            return;
        }
        if (x == mn) {
            if (!aux || aux->empty()) {mn = mx; return;}
            else mn = x = merge(aux->mn, m[aux->mn]->mn);
        } else if (x == mx) {
            if (!aux || aux->empty()) {mx = mn; return;}
            else mx = x = merge(aux->mx, m[aux->mx]->mx);
        }
        uint hi = high(x), lo = low(x);
        m[hi]->erase(lo);
        if (m[hi]->empty()) aux->erase(hi);
    }

    uint upper_bound(uint x) {
        if (empty() || x >= mx) return NO;
        if (x < mn) return mn;
        if (K == 1) return mx;
        uint hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->mx > lo) return merge(hi, m[hi]->upper_bound(lo));
        uint next_hi = aux ? aux->upper_bound(hi) : NO;
        return next_hi == NO ? mx : merge(next_hi, m[next_hi]->mn);
    }

    uint inverse_upper_bound(uint x) {
        if (empty() || x <= mn) return NO;
        if (x > mx) return mx;
        if (K == 1) return mn;
        uint hi = high(x), lo = low(x);
        if (m[hi] && !m[hi]->empty() && m[hi]->mn < lo) return merge(hi, m[hi]->inverse_upper_bound(lo));
        uint prev_hi = aux ? aux->inverse_upper_bound(hi) : NO;
        return prev_hi == NO ? mn : merge(prev_hi, m[prev_hi]->mx);
    }
};