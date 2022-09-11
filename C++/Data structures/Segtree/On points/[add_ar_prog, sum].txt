template<typename T>
struct segtree_on_points {

    uint a, U;
    vec<pair<T, T>> seg_gr;
    vec<T> sm;
    vec<T> psA;
    vec<T> psB;

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
        sm.resize(U * 2);
        psA.resize(U * 2);
        psB.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            sm[U + q] = 0;
        }
        for (uint q = U; --q;) {
            seg_gr[q] = {seg_gr[q << 1].F, seg_gr[q << 1 | 1].S};
            upd(q);
        }
    }

    inline constexpr T gsz(int v) {
        return seg_gr[v].S - seg_gr[v].F + 1;
    }

    void apply_add(int v, T a, T b) {
        psA[v] += a;
        psB[v] += b;
        sm[v] += a * gsz(v) * gsz(v) + gsz(v) * b;
    }

    void push(uint v) {
        apply_add(v << 1, psA[v], psB[v]);
        apply_add(v << 1 | 1, psA[v], psA[v] * gsz(v << 1) * 2 + psB[v]);
        psA[v] = psB[v] = 0;
    }

    void upd(uint v) {
        sm[v] = sm[v << 1] + sm[v << 1 | 1];
    }

    T seg_sum(T ql, T qr, uint v) {
        const T l = seg_gr[v].F, r = seg_gr[v].S;
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return sm[v];
        }
        push(v);
        const auto lf = seg_sum(ql, qr, v << 1);
        const auto rg = seg_sum(ql, qr, v << 1 | 1);
        return lf + rg;
    }

    void seg_add_progression(T ql, T qr, uint v, T a0, T b) {
        const T l = seg_gr[v].F, r = seg_gr[v].S;
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(v, b, 2 * a0 - b);
            return;
        }
        push(v);
        T md = seg_gr[v << 1].S;
        seg_add_progression(ql, qr, v << 1, a0, b);
        T llen = max((T)0, (T)(md - max(ql, l) + 1));
        seg_add_progression(ql, qr, v << 1 | 1, a0 + llen * b, b);
        upd(v);
    }

    T seg_sum(T ql, T qr) { return seg_sum(ql, qr, 1) / 2; }
    inline void seg_add_progression(T ql, T qr, T a0, T b) {seg_add_progression(ql, qr, 1, a0, b);}
};