template<typename T>
class segtree_on_points {

    const T NO = -1;                               //Change, if need
    const T INF = numeric_limits<T>::max();

    size_t a, U;
    vector<pair<T, T>> seg_gr;
    vector<T> sm;
    vector<T> mn;
    vector<T> mx;
    vector<T> ps_add;
    vector<T> ps_set;

    inline constexpr T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void apply_set(int v, T val) {
        sm[v] = val * gsz(v);
        mn[v] = val;
        mx[v] = val;
        ps_set[v] = val;
        ps_add[v] = 0;
    }

    void apply_add(int v, T val) {
        (ps_set[v] != NO ? ps_set : ps_add)[v] += val;
        sm[v] += val * gsz(v);
        mn[v] += val;
        mx[v] += val;
    }

    void push(size_t v) {
        if (ps_set[v] != NO) {
            apply_set(v << 1, ps_set[v]);
            apply_set(v << 1 | 1, ps_set[v]);
            ps_set[v] = NO;
        } else if (ps_add[v] != 0) {
            apply_add(v << 1, ps_add[v]);
            apply_add(v << 1 | 1, ps_add[v]);
            ps_add[v] = 0;
        }
    }

    void upd(size_t v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
        mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
        mx[v] = max(mx[v << 1], mx[v << 1 | 1]);
    }

    T seg_sum(T ql, T qr, size_t v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sm[v];
        push(v);
        const auto lf = seg_sum(ql, qr, v << 1);
        const auto rg = seg_sum(ql, qr, v << 1 | 1);
        return lf + rg;
    }

    T seg_min(T ql, T qr, size_t v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return mn[v];
        push(v);
        const auto lf = seg_min(ql, qr, v << 1);
        const auto rg = seg_min(ql, qr, v << 1 | 1);
        return min(lf, rg);
    }

    T seg_max(T ql, T qr, size_t v) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
        if (qr < l || r < ql) return -INF;
        if (ql <= l && r <= qr) return mx[v];
        push(v);
        const auto lf = seg_max(ql, qr, v << 1);
        const auto rg = seg_max(ql, qr, v << 1 | 1);
        return max(lf, rg);
    }

    void seg_add(T ql, T qr, size_t v, T val) {
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

    void seg_set(T ql, T qr, size_t v, T val) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
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
        mn.resize(U * 2, INF);
        mx.resize(U * 2, -INF);
        ps_add.resize(U * 2);
        ps_set.resize(U * 2, NO);
        for (size_t q = 0; q < a; ++q) {
            sm[U + q] = 0;
            mx[U + q] = 0;
            mn[U + q] = 0;
        }
        for (size_t q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].first, seg_gr[q << 1 | 1].second};
            upd(q);
        }
    }

    T seg_sum(T ql, T qr) {return seg_sum(ql, qr, 1);}
    T seg_max(T ql, T qr) {return seg_max(ql, qr, 1);}
    T seg_min(T ql, T qr) {return seg_min(ql, qr, 1);}
    void seg_add(T ql, T qr, T val) {seg_add(ql, qr, 1, val);}
    void seg_set(T ql, T qr, T val) {seg_set(ql, qr, 1, val);}
};