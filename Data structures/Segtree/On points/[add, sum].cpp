template<typename T>
class segtree_on_points {

    size_t n, U;
    vector<array<T, 2>> seg_gr;
    vector<T> sm;
    vector<T> ps_add;

    T gsz(int v) {
        return seg_gr[v][1] - seg_gr[v][0] + 1;
    }

    void apply_add(size_t v, T val) {
        sm[v] += val * gsz(v);
        ps_add[v] += val;
    }

    void push(size_t v) {
        if (ps_add[v] != 0) {
            apply_add(v << 1, ps_add[v]);
            apply_add(v << 1 | 1, ps_add[v]);
            ps_add[v] = 0;
        }
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
    }

    T seg_sum(T ql, T qr, size_t v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sm[v];
        push(v);
        const auto lf = seg_sum(ql, qr, v << 1);
        const auto rg = seg_sum(ql, qr, v << 1 | 1);
        return lf + rg;
    }

    void seg_add(T ql, T qr, size_t v, T val) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, val);
            return;
        }
        push(v);
        seg_add(ql, qr, v << 1, val);
        seg_add(ql, qr, v << 1 | 1, val);
        upd(v);
    }

public:
    segtree_on_points() = default;

    segtree_on_points(vector<T> points) {
        if (points.empty()) return;
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());
        vector<array<T, 2>> gr;
        gr.reserve(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            if (i && points[i - 1] + 1 < points[i]) {
                gr.push_back({points[i - 1] + 1, points[i] - 1});
            }
            gr.push_back({points[i], points[i]});
        }
        n = gr.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        seg_gr.resize(U * 2);
        for (size_t i = 0; i < n; ++i) seg_gr[U + i] = gr[i];
        for (size_t i = n; i < U; ++i) {
            seg_gr[U + i][0] = seg_gr[U + i][1] = gr[n - 1][1] + 1;
        }
        sm.resize(U * 2);
        ps_add.resize(U * 2);
        for (size_t i = 0; i < n; ++i) sm[U + i] = 0;
        for (size_t i = U; --i;) {
            seg_gr[i] = {seg_gr[i << 1][0], seg_gr[i << 1 | 1][1]};
            upd(i);
        }
    }

    T seg_sum(T l, T r) {return seg_sum(l, r, 1);}
    void seg_add(T l, T r, T val) {seg_add(l, r, 1, val);}
};
