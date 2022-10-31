template<typename T>
struct segtree_on_points {

    struct tag {

        T bl, br;

        tag() = default;

        friend inline void push_tag(tag &t, tag &l, tag &r) {
            //write push if need, or remove it
        }

        friend inline bool is_neutral(const tag &t) {
            //write neutral if need, or remove it
            return false;
        }

        friend inline void merge(const tag &l, const tag &r, tag &res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //write merge
        }

        friend inline tag merge(const tag &l, const tag &r) {
            tag res;
            merge(l, r, res);
            return res;
        }
    };

    tag neutral_tag;   //init neutral tag

    uint a, U;
    vec<tag> m;

    segtree_on_points() = default;

    segtree_on_points(vec<T> points) {
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
        m.resize(U * 2);
        for (int q = 0; q < a; ++q) {
            m[U + q].bl = gr[q].F;
            m[U + q].br = gr[q].S;
        }
        for (int q = a; q < U; ++q) m[U + q].bl = m[U + q].br = gr[a - 1].S + 1;      //////!!!!!!!
        for (uint q = 0; q < a; ++q) {
            tag &t = m[U + q];
            //write init of last layer
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
            m[q].bl = l.bl;
            m[q].br = r.br;
        }
    }

    void push(uint v) {
        push_tag(m[v], m[v << 1], m[v << 1 | 1]);
    }

    void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    tag seg_query(T ql, T qr, uint v) {
        const T l = m[v].bl, r = m[v].br;
        if (qr < l || r < ql) return neutral_tag;
        if (ql <= l && r <= qr) {
            return m[v];
        }
        push(v);
        const tag lf = seg_query(ql, qr, v << 1);
        const tag rg = seg_query(ql, qr, v << 1 | 1);
        return merge(lf, rg);
    }
    tag seg_query(T ql, T qr) { return seg_query(ql, qr, 1); }

    void seg_change(T ql, T qr, uint v) {
        const T l = m[v].bl, r = m[v].br;
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            //write seg_change function
            
            return;
        }
        push(v);
        seg_change(ql, qr, v << 1);
        seg_change(ql, qr, v << 1 | 1);
        upd(v);
    }
    inline void seg_change(T ql, T qr) { seg_change(ql, qr, 1); }
};