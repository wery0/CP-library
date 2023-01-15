template<typename T>
class segtree_on_points {

    struct tag {

        T bl, br;

        tag() = default;

        friend inline void push_tag(tag& t, tag& l, tag& r) {
            //Write push if need, or remove it
        }

        friend inline bool is_neutral(const tag& t) {
            //Write neutral if need, or remove it
            return false;
        }

        friend inline void merge(const tag& l, const tag& r, tag& res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //Write merge
        }

        friend inline tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }
    };

    tag neutral_tag;   //Init neutral tag

    uint a, U;
    vector<tag> m;

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

    void seg_change(T ql, T qr, uint v) {
        const T l = m[v].bl, r = m[v].br;
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
        sort(all(points));
        points.erase(unique(all(points)), points.end());
        vector<pair<T, T>> gr;
        for (int q = 0; q < points.size(); ++q) {
            if (q && points[q - 1] + 1 < points[q]) {
                gr.emplace_back(points[q - 1] + 1, points[q] - 1);
            }
            gr.emplace_back(points[q], points[q]);
        }
        a = gr.size();
        U = geq_pow2(a);
        m.resize(U * 2);
        for (int q = 0; q < a; ++q) {
            m[U + q].bl = gr[q].first;
            m[U + q].br = gr[q].second;
        }
        for (int q = a; q < U; ++q) {
            m[U + q].bl = m[U + q].br = gr[a - 1].second + 1;
        }
        for (uint q = 0; q < a; ++q) {
            tag& t = m[U + q];
            //Write init of last layer
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
            m[q].bl = l.bl;
            m[q].br = r.br;
        }
    }

    tag seg_query(T ql, T qr) {return seg_query(ql, qr, 1);}
    inline void seg_change(T ql, T qr) {seg_change(ql, qr, 1);}
};