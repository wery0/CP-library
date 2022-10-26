template<typename T>
struct area_of_rect_union {

    struct segtree_on_points {

        const T inf = numeric_limits<T>::max();

        uint a, U;
        vec<pair<T, T>> seg_gr;
        vec<T> cnt_mn;
        vec<int> mn, ps_add;

        segtree_on_points(vec<T>& points) {
            if (points.empty()) return;
            vec<pair<T, T>> gr; 
            gr.reserve(points.size() * 2);
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
            for (int q = a; q < U; ++q) seg_gr[U + q].F = seg_gr[U + q].S = gr[a - 1].S + 1;
            mn.resize(U * 2, inf);
            cnt_mn.resize(U * 2);
            ps_add.resize(U * 2);
            for (uint q = 0; q < a; ++q) {
                mn[U + q] = 0;
                cnt_mn[U + q] = gsz(U + q);
            }
            for (uint q = U; --q;) {
                seg_gr[q] = {seg_gr[q << 1].F, seg_gr[q << 1 | 1].S};
                upd(q);
            }
        }

        inline constexpr T gsz(int v) {
            return seg_gr[v].S - seg_gr[v].F + 1;
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
            const T l = seg_gr[v].F, r = seg_gr[v].S;
            if (qr < l || r < ql) return {inf, 1};
            if (ql <= l && r <= qr) {
                return {mn[v], cnt_mn[v]};
            }
            push(v);
            const auto lf = seg_min_cmin(ql, qr, v << 1);
            const auto rg = seg_min_cmin(ql, qr, v << 1 | 1);
            if (lf.F == rg.F) return {lf.F, lf.S + rg.S};
            return min(lf, rg);
        }

        void seg_add(T ql, T qr, uint v, T val) {
            const T l = seg_gr[v].F, r = seg_gr[v].S;
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

        pair<T, T> seg_min_cmin(T ql, T qr) { return seg_min_cmin(ql, qr, 1); }
        inline void seg_add(T ql, T qr, T val) { seg_add(ql, qr, 1, val); }
    };

    struct vseg {
        T x, y1, y2;
        bool is_left;
    };

    vec<vseg> store;
    vec<T> pts;

    area_of_rect_union(uint nrect = 0) {
        store.reserve(nrect * 2);
        pts.reserve(nrect * 2);
    }

    void add_rect(T x1, T y1, T x2, T y2) {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2 || y1 == y2) return;
        --y2;
        store.pb({x1, y1, y2, 1});
        store.pb({x2, y1, y2, 0});
        pts.pb(y1);
        pts.pb(y2);
    }

    ll calc_area() {
        if (store.empty()) return 0;
        ll mny = *min_element(all(pts));
        for (auto &i : pts) i -= mny;
        for (auto &i : store) i.y1 -= mny, i.y2 -= mny;
        unify(pts);
        segtree_on_points st(pts);
        sort(all(store), [&](const auto & v1, const auto & v2) {return v1.x < v2.x;});
        ll ans = 0, prx = 0;
        for (const auto &seg : store) {
            pll p = st.seg_min_cmin(0, pts.back());
            ans += 1ll * (pts.back() + 1 - (p.F == 0 ? p.S : 0)) * (seg.x - prx);
            st.seg_add(seg.y1, seg.y2, seg.is_left ? 1 : -1);
            prx = seg.x;
        }
        return ans;
    }
};
//Coordinates of rectangle endpoints are lattice points, i. e. rect(0, 0, 3, 3) has area 9.
//area_of_rect_union<coordinats_type> kek([nrect]);