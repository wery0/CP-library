template<typename X, typename Y>
class lichao_on_points {

    static constexpr Y inf = numeric_limits<Y>::max();

    struct func {
        Y k, b;

        func() = default;
        func(Y k, Y b): k(k), b(b) {}

        Y eval(X x) const {
            return k * x + b;
        }
    };

    size_t n, U;
    vector<X> bl, br;
    vector<func> vf;

    void add_seg(X ql, X qr, func nf, size_t v) {
        const X l = bl[v], r = br[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            add_line(nf, v);
            return;
        }
        add_seg(ql, qr, nf, v << 1);
        add_seg(ql, qr, nf, v << 1 | 1);
    }

    void add_line(func nf, size_t v) {
        while (true) {
            const X l = bl[v], r = br[v];
            Y vl_cur = vf[v].eval(l);
            Y vr_cur = vf[v].eval(r);
            Y vl_new = nf.eval(l);
            Y vr_new = nf.eval(r);
            if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
                if (vl_new < vl_cur) vf[v] = nf;
                return;
            }
            X md = br[v << 1];
            Y vmd_cur = vf[v].eval(md);
            Y vmd_new = nf.eval(md);
            if (vmd_new < vmd_cur) {
            	swap(vf[v], nf);
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
        vf.resize(U * 2, func(0, inf));
        for (size_t i = 0; i < n; ++i) bl[U + i] = br[U + i] = points[i];
        for (size_t i = n; i < U; ++i) bl[U + i] = br[U + i] = points[n - 1];
        for (size_t i = U; --i; ) {
            bl[i] = bl[i << 1];
            br[i] = br[i << 1 | 1];
        }
    }

    //O(log(|points|))
    Y get_min(X x) {
        size_t v = 1;
        Y o = vf[v].eval(x);
        while (v < U) {
            v <<= 1;
            v += x > br[v];
            o = min(o, vf[v].eval(x));
        }
        return o;
    }

    //O(log(|points|))
    void add_line(Y k, Y b) {add_line(func(k, b), 1);}

    //O(log(|points|) ^ 2)
    void add_seg(X l, X r, Y k, Y b) {add_seg(l, r, func(k, b), 1);}
};
//Usage: lichao_on_points<X, Y> kek(points), where
//X - type of coordinates,
//Y - type of K and B in linear function y = Kx + B,
//points - vector of possible values of X.
