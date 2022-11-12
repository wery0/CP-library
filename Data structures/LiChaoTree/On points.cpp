template<typename T>
struct lichao_on_points {

    static constexpr T inf = numeric_limits<T>::max();

    uint a, U;
    vec<T> bl, br;
    vec<T> vk, vb;

    lichao_on_points(vec<T> points) {
        if (points.empty()) points = {0};
        unify(points);
        a = points.size();
        U = geq_pow2(a);
        bl.resize(U * 2);
        br.resize(U * 2);
        vk.resize(U * 2);
        vb.resize(U * 2, inf);
        for (uint q = 0; q < a; ++q) bl[U + q] = br[U + q] = points[q];
        for (uint q = a; q < U; ++q) bl[U + q] = br[U + q] = points[a - 1];
        for (int q = U; --q; ) {
            bl[q] = bl[q << 1];
            br[q] = br[q << 1 | 1];
        }
    }

    void add_seg(T ql, T qr, T k, T b, uint v) {
        const T l = bl[v], r = br[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            add_line(k, b, v);
            return;
        }
        add_seg(ql, qr, k, b, v << 1);
        add_seg(ql, qr, k, b, v << 1 | 1);
    }

    void add_line(T k, T b, uint v) {
        for (;;) {
            const T l = bl[v], r = br[v];
            T vl_cur = vk[v] * l + vb[v];
            T vr_cur = vk[v] * r + vb[v];
            T vl_new = k * l + b;
            T vr_new = k * r + b;
            if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
                if (vl_new < vl_cur) vk[v] = k, vb[v] = b;
                return;
            }
            T md = br[v << 1];
            T vmd_cur = vk[v] * md + vb[v];
            T vmd_new = k * md + b;
            if (vmd_new < vmd_cur) {
                swap(vk[v], k), swap(vb[v], b);
                swap(vl_cur, vl_new);
            }
            v = vl_new < vl_cur ? v << 1 : v << 1 | 1;
        }
    }

    T get_min(T x) {
        uint v = 1;
        T o = vk[v] * x + vb[v];
        while (v < U) {
            v <<= 1;
            v += x > br[v];
            chmin(o, vk[v] * x + vb[v]);
        }
        return o;
    }

    void add_seg(T ql, T qr, T k, T b) {add_seg(ql, qr, k, b, 1);}
    void add_line(T k, T b) {add_line(k, b, 1);}
};
//Add line - O(log(N))
//Add seg - O(log(N)^2)