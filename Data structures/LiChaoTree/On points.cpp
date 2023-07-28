template<typename X, typename Y>
class lichao_on_points {

    static constexpr Y inf = numeric_limits<Y>::max();

    size_t n, U;
    vector<X> bl, br;
    vector<Y> vk, vb;

    void add_seg(X ql, X qr, Y k, Y b, size_t v) {
        const X l = bl[v], r = br[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            add_line(k, b, v);
            return;
        }
        add_seg(ql, qr, k, b, v << 1);
        add_seg(ql, qr, k, b, v << 1 | 1);
    }

    void add_line(Y k, Y b, size_t v) {
        while (true) {
            const X l = bl[v], r = br[v];
            Y vl_cur = vk[v] * l + vb[v];
            Y vr_cur = vk[v] * r + vb[v];
            Y vl_new = k * l + b;
            Y vr_new = k * r + b;
            if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
                if (vl_new < vl_cur) vk[v] = k, vb[v] = b;
                return;
            }
            X md = br[v << 1];
            Y vmd_cur = vk[v] * md + vb[v];
            Y vmd_new = k * md + b;
            if (vmd_new < vmd_cur) {
                swap(vk[v], k), swap(vb[v], b);
                swap(vl_cur, vl_new);
            }
            v = vl_new < vl_cur ? v << 1 : v << 1 | 1;
        }
    }

public:
    lichao_on_points() = default;
    lichao_on_points(vector<X> points) {
        if (points.empty()) points = {0};
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());
        n = points.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        bl.resize(U * 2);
        br.resize(U * 2);
        vk.resize(U * 2);
        vb.resize(U * 2, inf);
        for (size_t i = 0; i < n; ++i) bl[U + i] = br[U + i] = points[i];
        for (size_t i = n; i < U; ++i) bl[U + i] = br[U + i] = points[n - 1];
        for (size_t i = U; --i; ) {
            bl[i] = bl[i << 1];
            br[i] = br[i << 1 | 1];
        }
    }

    Y get_min(X x) {
        size_t v = 1;
        Y o = vk[v] * x + vb[v];
        while (v < U) {
            v <<= 1;
            v += x > br[v];
            o = min(o, vk[v] * x + vb[v]);
        }
        return o;
    }

    //O(log(|points|))
    void add_line(Y k, Y b) {add_line(k, b, 1);}

    //O(log(|points|) ^ 2)
    void add_seg(X l, X r, Y k, Y b) {add_seg(l, r, k, b, 1);}
};
//Usage: lichao_on_points<X, Y> kek(points), where
//X - type of coordinates,
//Y - type of K and B in linear function y = Kx + B,
//points - vector of possible values of X.
