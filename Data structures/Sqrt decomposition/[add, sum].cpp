template<typename T>
class seg_sum_seg_add {
    size_t n;
    size_t B;
    vector<int> st;
    vector<T> m;
    vector<T> bsm;
    vector<T> ps;

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
        ps.resize(nB);
    }

    //O(B + n / B)
    T seg_sum(size_t l, size_t r) const {
        assert(l <= r && r < n);
        auto stupid = [&](size_t b, size_t l, size_t r) {
            return accumulate(m.begin() + l, m.begin() + r + 1, T(0)) + ps[b] * (r - l + 1);
        };
        size_t bl = l / B;
        size_t br = r / B;
        if (bl == br) return stupid(bl, l, r);
        else {
            return stupid(bl, l, st[bl + 1] - 1) +
                   stupid(br, st[br], r) +
                   accumulate(bsm.begin() + bl + 1, bsm.begin() + br, T(0)) +
                   accumulate(ps.begin() + bl + 1, ps.begin() + br, T(0)) * B;
        }
    }

    //O(B + n / B)
    void seg_add(size_t l, size_t r, T x) {
        assert(l <= r && r < n);
        auto stupid = [&](size_t b, size_t l, size_t r) {
            for (; l <= r; ++l) m[l] += x;
            bsm[b] += (r - l + 1) * x;
        };
        size_t bl = l / B;
        size_t br = r / B;
        if (bl == br) stupid(bl, l, r);
        else {
            stupid(bl, l, st[bl + 1] - 1);
            stupid(br, st[br], r);
            while (++bl < br) {
                ps[bl] += x;
                bsm[bl] += x * B;
            }
        }
    }

    T operator[](size_t i) const {return m[i] + ps[i / B];}
    void point_set(size_t i, T x) {
        size_t b = i / B;
        bsm[b] += x - (m[i] + ps[b]);
        m[i] = x - ps[b];
    }
};
