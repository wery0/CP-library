template<typename T>
class segtree_on_points {

    static constexpr T inf = numeric_limits<T>::max();

    size_t n, U;
    vector<pair<T, T>> seg_gr;
    vector<T> sm, mn, cnt_mn;
    vector<T> ps_add;

    inline constexpr T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void apply_add(size_t v, T val) {
        sm[v] += val * gsz(v);
        mn[v] += val;
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
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        cnt_mn[v] = (mn[v << 1] == mn[v] ? cnt_mn[v << 1] : 0) +
                    (mn[v << 1 | 1] == mn[v] ? cnt_mn[v << 1 | 1] : 0);
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
        if (qr < l || r < ql) return inf;
        if (ql <= l && r <= qr) return mn[v];
        push(v);
        const auto lf = seg_min(ql, qr, v << 1);
        const auto rg = seg_min(ql, qr, v << 1 | 1);
        return min(lf, rg);
    }

    pair<T, T> seg_min_cmin(T ql, T qr, size_t v) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return {inf, 1};
        if (ql <= l && r <= qr) return {mn[v], cnt_mn[v]};
        push(v);
        const auto lf = seg_min_cmin(ql, qr, v << 1);
        const auto rg = seg_min_cmin(ql, qr, v << 1 | 1);
        if (lf.first == rg.first) return {lf.first, lf.second + rg.second};
        return min(lf, rg);
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
        vector<pair<T, T>> gr;
        gr.reserve(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            if (i && points[i - 1] + 1 < points[i]) {
                gr.emplace_back(points[i - 1] + 1, points[i] - 1);
            }
            gr.emplace_back(points[i], points[i]);
        }
        n = gr.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        seg_gr.resize(U * 2);
        for (size_t i = 0; i < n; ++i) seg_gr[U + i] = gr[i];
        for (size_t i = n; i < U; ++i) {
            seg_gr[U + i].first = seg_gr[U + i].second = gr[n - 1].second + 1;
        }
        sm.resize(U * 2);
        mn.resize(U * 2, inf);
        cnt_mn.resize(U * 2);
        ps_add.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            sm[U + i] = 0;
            mn[U + i] = 0;
            cnt_mn[U + i] = gsz(U + i);
        }
        for (size_t i = U; --i;) {
            seg_gr[i] = {seg_gr[i << 1].first, seg_gr[i << 1 | 1].second};
            upd(i);
        }
    }

    T seg_sum(T ql, T qr) {return seg_sum(ql, qr, 1);}
    T seg_min(T ql, T qr) {return seg_min(ql, qr, 1);}
    pair<T, T> seg_min_cmin(T ql, T qr) {return seg_min_cmin(ql, qr, 1);}
    void seg_add(T ql, T qr, T val) {seg_add(ql, qr, 1, val);}
};