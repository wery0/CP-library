template<typename T>
class segtree_on_points {

    size_t a, U;
    vector<pair<T, T>> seg_gr;
    vector<T> sm;
    vector<T> psA;
    vector<T> psB;

    inline constexpr T gsz(int v) {
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

    void seg_add_progression(T ql, T qr, size_t v, T a0, T b) {
        const T l = seg_gr[v].first, r = seg_gr[v].second;
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
        psA.resize(U * 2);
        psB.resize(U * 2);
        for (size_t q = 0; q < a; ++q) {
            sm[U + q] = 0;
        }
        for (size_t q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].first, seg_gr[q << 1 | 1].second};
            upd(q);
        }
    }

    T seg_sum(T ql, T qr) {return seg_sum(ql, qr, 1) / 2;}
    inline void seg_add_progression(T ql, T qr, T a0, T b) {seg_add_progression(ql, qr, 1, a0, b);}
};