const ll mod = 998244353;
struct segtree_point_upd {

    uint a, U;
    vec<uint> A;
    vec<uint> B;

    segtree_point_upd() = default;

    segtree_point_upd(vec<pii> &n) {
        a = n.size();
        U = geq_pow2(a);
        A.resize(U * 2, 1);
        B.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            A[U + q] = n[q].F;
            B[U + q] = n[q].S;
        }
        for (uint q = U; --q;) {
            A[q] = 1ll * A[q << 1] * A[q << 1 | 1] % mod;
            B[q] = (B[q << 1 | 1] + 1ll * A[q << 1 | 1] * B[q << 1]) % mod;
        }
    }

    inline void upd(uint q) {
        A[q] = 1ll * A[q << 1] * A[q << 1 | 1] % mod;
        B[q] = (B[q << 1 | 1] + 1ll * A[q << 1 | 1] * B[q << 1]) % mod;
    }

    inline uint seg_composite(uint ql, uint qr, uint x) {
        ql += U, qr += U;
        ull lA = 1, lB = 0;
        ull rA = 1, rB = 0;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) lA = lA * A[ql] % mod, lB = (B[ql] + A[ql] * lB) % mod;
            if (~qr & 1) rB = (rB + rA * B[qr]) % mod, rA = rA * A[qr] % mod;
        }
        lA = lA * rA % mod, lB = (rB + rA * lB) % mod;
        return (lA * x + lB) % mod;
    }

    inline void change(uint p, uint c, uint d) {
        p += U;
        A[p] = c;
        B[p] = d;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//Stores a[i] and b[i], representing a function f_i(x) = a[i] * x + b[i]
//Can calculate f_r(f_{r-1}(...f_l(x))) by modulo
//https://judge.yosupo.jp/problem/point_set_range_composite