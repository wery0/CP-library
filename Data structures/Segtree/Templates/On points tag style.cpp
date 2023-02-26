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

    size_t n, U;
    vector<tag> m;

    void push(size_t v) {
        push_tag(m[v], m[v << 1], m[v << 1 | 1]);
    }

    void upd(size_t v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

    tag seg_query(T ql, T qr, size_t v) {
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

    void seg_change(T ql, T qr, size_t v) {
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
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());
        vector<pair<T, T>> gr;
        for (size_t i = 0; i < points.size(); ++i) {
            if (i && points[i - 1] + 1 < points[i]) {
                gr.emplace_back(points[i - 1] + 1, points[i] - 1);
            }
            gr.emplace_back(points[i], points[i]);
        }
        n = gr.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        m.resize(U * 2);
        for (size_t i = 0; i < n; ++i) {
            m[U + i].bl = gr[i].first;
            m[U + i].br = gr[i].second;
        }
        for (size_t i = n; i < U; ++i) {
            m[U + i].bl = m[U + i].br = gr[n - 1].second + 1;
        }
        for (size_t i = 0; i < n; ++i) {
            tag& t = m[U + i];
            //Write init of last layer
        }
        for (size_t i = U; --i;) {
            const tag &l = m[i << 1], &r = m[i << 1 | 1];
            merge(l, r, m[i]);
            m[i].bl = l.bl;
            m[i].br = r.br;
        }
    }

    tag seg_query(T ql, T qr) {return seg_query(ql, qr, 1);}
    void seg_change(T ql, T qr) {seg_change(ql, qr, 1);}
};