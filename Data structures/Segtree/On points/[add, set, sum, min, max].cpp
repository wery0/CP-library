template<typename T>
class segtree_on_points {

    static constexpr T INF = numeric_limits<T>::max();

    size_t n, U;
    vector<array<T, 2>> seg_gr;
    vector<T> sm, mn, mx;
    vector<T> ps;
    vector<char> is_set;

    T gsz(int v) {
        return seg_gr[v][1] - seg_gr[v][0] + 1;
    }

    void apply_set(int v, T val) {
        sm[v] = val * gsz(v);
        mn[v] = val;
        mx[v] = val;
        ps[v] = val;
        is_set[v] = 1;
    }

    void apply_add(int v, T val) {
        ps[v] += val;
        sm[v] += val * gsz(v);
        mn[v] += val;
        mx[v] += val;
    }

    void push(size_t v) {
        if (is_set[v]) {
            apply_set(v << 1, ps[v]);
            apply_set(v << 1 | 1, ps[v]);
            ps[v] = 0;
            is_set[v] = 0;
        } else if (ps[v]) {
            apply_add(v << 1, ps[v]);
            apply_add(v << 1 | 1, ps[v]);
            ps[v] = 0;
        }
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
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

    T seg_min(T ql, T qr, size_t v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return mn[v];
        push(v);
        const auto lf = seg_min(ql, qr, v << 1);
        const auto rg = seg_min(ql, qr, v << 1 | 1);
        return min(lf, rg);
    }

    T seg_max(T ql, T qr, size_t v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return -INF;
        if (ql <= l && r <= qr) return mx[v];
        push(v);
        const auto lf = seg_max(ql, qr, v << 1);
        const auto rg = seg_max(ql, qr, v << 1 | 1);
        return max(lf, rg);
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

    void seg_set(T ql, T qr, size_t v, T val) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_set(v, val);
            return;
        }
        push(v);
        seg_set(ql, qr, v << 1, val);
        seg_set(ql, qr, v << 1 | 1, val);
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
        mn.resize(U * 2, INF);
        mx.resize(U * 2, -INF);
        ps.resize(U * 2);
        is_set.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            sm[U + i] = 0;
            mx[U + i] = 0;
            mn[U + i] = 0;
        }
        for (size_t i = U; --i;) {
            seg_gr[i] = {seg_gr[i << 1][0], seg_gr[i << 1 | 1][1]};
            upd(i);
        }
    }

    T seg_sum(T l, T r) {return seg_sum(l, r, 1);}
    T seg_min(T l, T r) {return seg_min(l, r, 1);}
    T seg_max(T l, T r) {return seg_max(l, r, 1);}
    void seg_add(T l, T r, T val) {seg_add(l, r, 1, val);}
    void seg_set(T l, T r, T val) {seg_set(l, r, 1, val);}
};
