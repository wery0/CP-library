template<typename T>
class segtree_on_points {

    size_t n, U;
    vector<pair<T, T>> seg_gr;
    //Create needed vectors

    inline T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void push(size_t v) {
        //Write push, if need
    }

    void upd(size_t v) {
        //Write update
    }

    T seg_statistic(T ql, T qr, size_t v) {
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

    void seg_change(T ql, T qr, size_t v) {
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
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());
        vector<pair<T, T>> gr;
        for (size_t i = 0; i < points.size(); ++i) {
            if (i && points[i - 1] + 1 < points[i]) {
                gr.emplace_back(points[i - 1] + 1, points[i] - 1);
            }
            gr.emplace_back(points[i], points[i]);
        }
        n = gr.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        seg_gr.resize(U * 2, {gr[n - 1].second + 1, gr[n - 1].second + 1});
        for (size_t i = 0; i < n; ++i) seg_gr[U + i] = gr[i];
        //Resize needed vectors with U * 2

        for (size_t i = 0; i < n; ++i) {
            //Write init of last layer. Indices of last layer is U + i.

        }
        for (size_t i = U; --i;) {
            seg_gr[i] = {seg_gr[i << 1].first, seg_gr[i << 1 | 1].second};
            upd(i);
            //Write additional info, if need
        }
    }

    T seg_statistic(T ql, T qr) {return seg_statistic(ql, qr, 1);}
    void seg_change(T ql, T qr) {seg_change(ql, qr, 1);}
};