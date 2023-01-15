template<typename T>
class area_of_rect_union {

    struct segtree_on_points {

        const T inf = numeric_limits<T>::max();

        uint a, U;
        vector<pair<T, T>> seg_gr;
        vector<T> cnt_mn;
        vector<int> mn, ps_add;

        segtree_on_points(vector<T>& points) {
            if (points.empty()) return;
            vector<pair<T, T>> gr;
            gr.reserve(points.size() * 2);
            for (int q = 0; q < points.size(); ++q) {
                if (q && points[q - 1] + 1 < points[q]) {
                    gr.emplace_back(points[q - 1] + 1, points[q] - 1);
                }
                gr.emplace_back(points[q], points[q]);
            }
            a = gr.size();
            U = geq_pow2(a);
            seg_gr.resize(U * 2, {gr[a - 1].second + 1, gr[a - 1].second + 1});
            for (int q = 0; q < a; ++q) seg_gr[U + q] = gr[q];
            mn.resize(U * 2, inf);
            cnt_mn.resize(U * 2);
            ps_add.resize(U * 2);
            for (uint q = 0; q < a; ++q) {
                mn[U + q] = 0;
                cnt_mn[U + q] = gsz(U + q);
            }
            for (uint q = U; --q;) {
                seg_gr[q] = {seg_gr[q << 1].first, seg_gr[q << 1 | 1].second};
                upd(q);
            }
        }

        inline T gsz(int v) {
            return seg_gr[v].second - seg_gr[v].first + 1;
        }

        void apply_add(uint v, T val) {
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
            mn[v] = min(mn[v << 1], mn[v << 1 | 1]);
            cnt_mn[v] = (mn[v << 1] == mn[v] ? cnt_mn[v << 1] : 0) +
                        (mn[v << 1 | 1] == mn[v] ? cnt_mn[v << 1 | 1] : 0);
        }

        pair<T, T> seg_min_cmin(T ql, T qr, uint v) {
            const auto [l, r] = seg_gr[v];
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

        pair<T, T> seg_min_cmin(T ql, T qr) {return seg_min_cmin(ql, qr, 1);}
        inline void seg_add(T ql, T qr, T val) {seg_add(ql, qr, 1, val);}
    };

    struct vseg {
        T x, y1, y2;
        bool is_left;
    };

    vector<vseg> store;
    vector<T> pts;

public:
    area_of_rect_union(uint nrect = 0) {
        store.reserve(nrect * 2);
        pts.reserve(nrect * 2);
    }

    void add_rect(T x1, T y1, T x2, T y2) {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2 || y1 == y2) return;
        --y2;
        store.emplace_back(x1, y1, y2, 1);
        store.emplace_back(x2, y1, y2, 0);
        pts.push_back(y1);
        pts.push_back(y2);
    }

    //O(nlog(n)), where n is amount of added rectangles
    ll calc_area() {
        if (store.empty()) return 0;
        unify(pts);
        segtree_on_points st(pts);
        sort(all(store), [&](const auto& v1, const auto& v2) {return v1.x < v2.x;});
        ll ans = 0, prx = 0;
        for (const auto& seg : store) {
            auto [mn, cmn] = st.seg_min_cmin(pts[0], pts.back());
            ans += 1ll * (pts.back() - pts[0] + 1 - (mn == 0 ? cmn : 0)) * (seg.x - prx);
            st.seg_add(seg.y1, seg.y2, seg.is_left ? 1 : -1);
            prx = seg.x;
        }
        return ans;
    }
};
//Coordinates of rectangle endpoints are lattice points, i. e. rect(0, 0, 3, 3) has area 9.
//Usage: area_of_rect_union<coordinates_type> kek([nrect]);