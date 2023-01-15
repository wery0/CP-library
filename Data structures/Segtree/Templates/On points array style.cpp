template<typename T>
class segtree_on_points {

    uint a, U;
    vector<pair<T, T>> seg_gr;
    //Create needed vectors

    inline T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void push(uint v) {
        //Write push, if need
    }

    void upd(uint v) {
        //Write update
    }

    T seg_statistic(T ql, T qr, uint v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return neutral_element;
        if (ql <= l && r <= qr) {
            //Return needed statistic

        }
        push(v);
        const auto lf = seg_statistic(ql, qr, v << 1);
        const auto rg = seg_statistic(ql, qr, v << 1 | 1);
        return ?;
    }

    void seg_change(T ql, T qr, uint v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //Write seg_change function

            return;
        }
        push(v);
        seg_change(ql, qr, v << 1);
        seg_change(ql, qr, v << 1 | 1);
        upd(v);
    }

public:
    segtree_on_points() = default;
    segtree_on_points(vector<T> points) {
        if (points.empty()) return;
        sort(all(points));
        points.erase(unique(all(points)), points.end());
        vector<pair<T, T>> gr;
        for (int q = 0; q < points.size(); ++q) {
            if (q && points[q - 1] + 1 < points[q]) {
                gr.emplace_back(points[q - 1] + 1, points[q] - 1);
            }
            gr.emplace_back(points[q], points[q]);
        }
        a = gr.size();
        U = geq_pow2(a);
        seg_gr.resize(U * 2, {gr[a - 1].second + 1, gr[a - 1].second + 1});
        for (int q = 0; q < a; ++q) seg_gr[U + q] = gr[q];
        //Resize needed vectors with U * 2

        for (uint q = 0; q < a; ++q) {
            //Write init of last layer. Indices of last layer is U + q.

        }
        for (uint q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].first, seg_gr[q << 1 | 1].second};
            upd(q);
            //Write additional info, if need
        }
    }

    T seg_statistic(T ql, T qr) {return seg_statistic(ql, qr, 1);}
    void seg_change(T ql, T qr) {seg_change(ql, qr, 1);}
};