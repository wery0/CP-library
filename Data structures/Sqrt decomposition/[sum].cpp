template<typename T>
class seg_sum {
    size_t n;
    size_t B;
    vector<int> st;
    vector<T> m;
    vector<T> bsm;

public:
    seg_sum_seg_add() = default;
    template<typename I>
    seg_sum_seg_add(I first, I last): n(std::distance(first, last)), B(sqrtl(n)), m(first, last) {
        size_t nB = (n + B - 1) / B;
        st.resize(nB + 1, n);
        for (int i = 0; i < nB; ++i) st[i] = B * i;
        bsm.resize(nB);
        for (int i = 0; i < nB; ++i) {
            bsm[i] = accumulate(m.begin() + st[i], m.begin() + st[i + 1], T(0));
        }
    }

    //O(B + n / B)
    T seg_sum(size_t l, size_t r) const {
        assert(l <= r && r < n);
        auto stupid = [&](size_t b, size_t l, size_t r) {
            return accumulate(m.begin() + l, m.begin() + r + 1, T(0));
        };
        size_t bl = l / B;
        size_t br = r / B;
        if (bl == br) return stupid(bl, l, r);
        else {
            return stupid(bl, l, st[bl + 1] - 1) +
                   stupid(br, st[br], r) +
                   accumulate(bsm.begin() + bl + 1, bsm.begin() + br, T(0));
        }
    }

    T operator[](size_t i) const {return m[i];}
    void point_set(size_t i, T x) {
        size_t b = i / B;
        bsm[b] += x - m[i];
        m[i] = x;
    }
};
