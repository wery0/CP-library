//Stores a[i] and b[i], representing a function f_i(x) = a[i] * x + b[i]
//https://judge.yosupo.jp/problem/point_set_range_composite
template<typename T>
class segtree_point_upd {

    size_t n;
    vector<T> A;
    vector<T> B;

    void upd(size_t v) {
        A[v] = A[v << 1] * A[v << 1 | 1];
        B[v] = B[v << 1 | 1] + A[v << 1 | 1] * B[v << 1];
    }

public:
    segtree_point_upd() = default;

    segtree_point_upd(vector<pair<T, T>>& arr): n(arr.size()) {
        if (!n) return;
        A.resize(n * 2, 1);
        B.resize(n * 2);
        for (size_t i = 0; i < n; ++i) {
            A[n + i] = arr[i].first;
            B[n + i] = arr[i].second;
        }
        for (size_t i = n; --i;) upd(i);
    }

    //Calculates f_r(f_{r-1}(...f_l(x)))
    T seg_composite(size_t l, size_t r, T x) const {
        l += n, r += n;
        T lA = 1, lB = 0;
        T rA = 1, rB = 0;
        for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
            if (l & 1) lA *= A[l], lB = B[l] + A[l] * lB;
            if (~r & 1) rB = rB + rA * B[r], rA *= A[r];
        }
        lA *= rA, lB = rB + rA * lB;
        return lA * x + lB;
    }

    void point_change(size_t pos, T a, T b) {
        pos += n;
        A[pos] = a, B[pos] = b;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};