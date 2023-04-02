template<typename T>
class segtree_on_points {

    size_t n, U;
    vector<pair<T, T>> seg_gr;
    vector<T> sm;
    vector<T> psA, psB;

    T gsz(int v) {
        return seg_gr[v].second - seg_gr[v].first + 1;
    }

    void apply_add(int v, T a, T b) {
        psA[v] += a;
        psB[v] += b;
        sm[v] += a * gsz(v) * gsz(v) + gsz(v) * b;
    }

    void push(size_t v) {
        apply_add(v << 1, psA[v], psB[v]);
        apply_add(v << 1 | 1, psA[v], psA[v] * gsz(v << 1) * 2 + psB[v]);
        psA[v] = psB[v] = 0;
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

    void seg_add_progression(T ql, T qr, size_t v, T a0, T b) {
        const auto [l, r] = seg_gr[v];
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, b, 2 * a0 - b);
            return;
        }
        push(v);
        T md = seg_gr[v << 1].second;
        seg_add_progression(ql, qr, v << 1, a0, b);
        T llen = max((T)0, (T)(md - max(ql, l) + 1));
        seg_add_progression(ql, qr, v << 1 | 1, a0 + llen * b, b);
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
        psA.resize(U * 2);
        psB.resize(U * 2);
        for (size_t i = 0; i < n; ++i) sm[U + i] = 0;
        for (size_t i = U; --i;) {
            seg_gr[i] = {seg_gr[i << 1].first, seg_gr[i << 1 | 1].second};
            upd(i);
        }
    }

    T seg_sum(T ql, T qr) {return seg_sum(ql, qr, 1) / 2;}
    void seg_add_progression(T ql, T qr, T a0, T b) {seg_add_progression(ql, qr, 1, a0, b);}
};