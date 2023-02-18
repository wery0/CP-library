template<typename T>
class segtree_on_points {

    const T inf = numeric_limits<T>::max();

    uint a, U;
    vector<pair<T, T>> seg_gr;
    vector<T> sm;
    vector<T> mn, cnt_mn;
    vector<T> ps_add;

    inline constexpr T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void apply_add(uint v, T val) {
        sm[v] += val * gsz(v);
        mn[v] += val;
        ps_add[v] += val;
    }

    void push(uint v) {
        if (ps_add[v] != 0) {
            apply_add(v << 1, ps_add[v]);
            apply_add(v << 1 | 1, ps_add[v]);
            ps_add[v] = 0;
        }
    }

    void upd(uint v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        cnt_mn[v] = (mn[v << 1] == mn[v] ? cnt_mn[v << 1] : 0) +
                    (mn[v << 1 | 1] == mn[v] ? cnt_mn[v << 1 | 1] : 0);
    }

    T seg_sum(T ql, T qr, uint v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return sm[v];
        }
        push(v);
        const auto lf = seg_sum(ql, qr, v << 1);
        const auto rg = seg_sum(ql, qr, v << 1 | 1);
        return lf + rg;
    }

    T seg_min(T ql, T qr, uint v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return inf;
        if (ql <= l && r <= qr) {
            return mn[v];
        }
        push(v);
        const auto lf = seg_min(ql, qr, v << 1);
        const auto rg = seg_min(ql, qr, v << 1 | 1);
        return min(lf, rg);
    }

    pair<T, T> seg_min_cmin(T ql, T qr, uint v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return {inf, 1};
        if (ql <= l && r <= qr) {
            return {mn[v], cnt_mn[v]};
        }
        push(v);
        const auto lf = seg_min_cmin(ql, qr, v << 1);
        const auto rg = seg_min_cmin(ql, qr, v << 1 | 1);
        if (lf.first == rg.first) return {lf.first, lf.second + rg.second};
        return min(lf, rg);
    }

    void seg_add(T ql, T qr, uint v, T val) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
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
        for (int q = 0; q < points.size(); ++q) {
            if (q && points[q - 1] + 1 < points[q]) {
                gr.emplace_back(points[q - 1] + 1, points[q] - 1);
            }
            gr.emplace_back(points[q], points[q]);
        }
        a = gr.size();
        U = geq_pow2(a);
        seg_gr.resize(U * 2);
        for (int q = 0; q < a; ++q) seg_gr[U + q] = gr[q];
        for (int q = a; q < U; ++q) seg_gr[U + q].first = seg_gr[U + q].second = gr[a - 1].second + 1;
        sm.resize(U * 2);
        mn.resize(U * 2, inf);
        cnt_mn.resize(U * 2);
        ps_add.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            sm[U + q] = 0;
            mn[U + q] = 0;
            cnt_mn[U + q] = gsz(U + q);
        }
        for (uint q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].first, seg_gr[q << 1 | 1].second};
            upd(q);
        }
    }

    T seg_sum(T ql, T qr) {return seg_sum(ql, qr, 1);}
    T seg_min(T ql, T qr) {return seg_min(ql, qr, 1);}
    pair<T, T> seg_min_cmin(T ql, T qr) {return seg_min_cmin(ql, qr, 1);}
    void seg_add(T ql, T qr, T val) {seg_add(ql, qr, 1, val);}
};