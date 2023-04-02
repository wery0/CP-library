template<typename T>
class segtree_point_upd {

    size_t n, U;
    vector<T> A;
    vector<T> B;

    void upd(size_t v) {
        A[v] = A[v << 1] * A[v << 1 | 1];
        B[v] = B[v << 1 | 1] + A[v << 1 | 1] * B[v << 1];
    }

public:
    segtree_point_upd() = default;

    segtree_point_upd(vector<pair<int, int>>& arr): n(arr.size()), U(n & (n - 1) ? 2 << __lg(n) : n) {
        if (!n) return;
        A.resize(U * 2, 1);
        B.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            A[U + i] = arr[i].first;
            B[U + i] = arr[i].second;
        }
        for (size_t i = U; --i;) upd(i);
    }

    T seg_composite(size_t ql, size_t qr, T x) const {
        ql += U, qr += U;
        T lA = 1, lB = 0;
        T rA = 1, rB = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) lA *= A[ql], lB = B[ql] + A[ql] * lB;
            if (~qr & 1) rB = rB + rA * B[qr], rA *= A[qr];
        }
        lA *= rA, lB = rB + rA * lB;
        return lA * x + lB;
    }

    void point_change(size_t pos, T c, T d) {
        pos += U;
        A[pos] = c, B[pos] = d;
        for (pos >>= 1; pos; pos >>= 1) upd(pos);
    }
};
//Stores a[i] and b[i], representing a function f_i(x) = a[i] * x + b[i]
//Can calculate f_r(f_{r-1}(...f_l(x)))
//https://judge.yosupo.jp/problem/point_set_range_composite