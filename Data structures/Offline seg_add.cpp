template<typename T>
struct offline_seg_add {

    int a;
    vec<T> m;

    offline_seg_add() = default;

    template<typename I>
    offline_seg_add(I first, I last) {
        a = last - first;
        m.resize(a + 1);
        for (int q = 0; q < a; ++q) {
            m[q] += *(first + q);
            m[q + 1] -= *(first + q);
        }
    }

    template<typename T_arr>
    offline_seg_add(T_arr &n) {
        (*this) = offline_seg_add(all(n));
    }

    offline_seg_add(int _a) {
        a = _a;
        m.resize(a + 1);
    }

    void seg_add(int l, int r, T val) {
        m[l] += val;
        m[r + 1] -= val;
    }

    void clear() {
        fill(all(m), 0);
    }

    void get_result(vec<T> &n) {
        assert(a <= n.size());
        n[0] = m[0];
        for (int q = 1; q < a; ++q) {
            n[q] = n[q - 1] + m[q];
        }
    }

    vec<T> get_result() {
        vec<T> n(a);
        get_result(n);
        return n;
    }
};