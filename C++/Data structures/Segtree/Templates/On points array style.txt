template<typename T>
struct segtree_on_points {

    uint a, U;
    vec<pair<T, T>> seg_gr;
    //create needed vectors

    segtree_on_points() = default;

    segtree_on_points(vec<T> points) {
        if (points.empty()) return;
        unify(points);
        vec<pair<T, T>> gr;
        for (int q = 0; q < points.size(); ++q) {
            if (q && points[q - 1] + 1 < points[q]) {
                gr.pb({points[q - 1] + 1, points[q] - 1});
            }
            gr.pb({points[q], points[q]});
        }
        a = gr.size();
        U = geq_pow2(a);
        seg_gr.resize(U * 2);
        for (int q = 0; q < a; ++q) seg_gr[U + q] = gr[q];
        for (int q = a; q < U; ++q) seg_gr[U + q].F = seg_gr[U + q].S = gr[a - 1].S + 1;  //!!!!!!!
        //resize need vectors with U * 2
        
        for (uint q = 0; q < a; ++q) {
            //write init of last layer. Indices of last layer is U + q.

        }
        for (uint q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].F, seg_gr[q << 1 | 1].S};
            upd(q);
            //write additional info, of need
        }
    }

    inline constexpr T gsz(int v) {
        return seg_gr[v].S - seg_gr[v].F + 1;
    }

    void push(uint v) {
        //write push, if need
    }

    void upd(uint v) {
        //write update
    }

    T seg_statistic(T ql, T qr, uint v) {
        const T l = seg_gr[v].F, r = seg_gr[v].S;
        if (qr < l || r < ql) return neutral_element;
        if (ql <= l && r <= qr) {
            //return needed statistic

        }
        push(v);
        const auto lf = seg_statistic(ql, qr, v << 1);
        const auto rg = seg_statistic(ql, qr, v << 1 | 1);
        return ?;
    }
    T seg_statistic(T ql, T qr) { return seg_statistic(ql, qr, 1); }

    void seg_change(T ql, T qr, uint v) {
        const T l = seg_gr[v].F, r = seg_gr[v].S;
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //write seg_change function
            
            return;
        }
        push(v);
        seg_change(ql, qr, v << 1);
        seg_change(ql, qr, v << 1 | 1);
        upd(v);
    }
    inline void seg_change(T ql, T qr) { seg_change(ql, qr, 1); }
};