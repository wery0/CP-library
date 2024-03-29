//T - type of coordinates
//D - number of dimensions
/*
AXIS_MODE:
On each level of recursion points are splitting by
  CYCLIC: consecutive axes (0, 1, ... D - 1, 0, 1, ...)
  LONGEST: axis with the longest projection of all points
*/
//IS_SQUARED_METRIC - indicator of whether the metric returns the square of the distance or not
enum AXIS_MODE {CYCLIC, LONGEST};
template<typename T, const int D, const AXIS_MODE MODE = LONGEST, const bool IS_SQUARED_METRIC = true>
class KD_tree {
    static_assert(MODE == 0 || MODE == 1);
    static_assert(is_signed_v<T>);

    struct hyperrect {array<T, D> lf, rg;};

    using point = array<T, D>;
    //Use this instead when you want to store some info with points
    // struct point {
    //     array<T, D> p;
    //     T& operator[](size_t i) {return p[i];}
    //     const T& operator[](size_t i) const {return p[i];}
    // };

    //Change, if need.
    const T EPS = is_integral_v<T> ? 0 : 1e-9;

    size_t n;
    vector<point> m;
    vector<T> mid_axis;
    vector<size_t> vaxis;
    vector<hyperrect> bounding_box;
    point pnt;

    bool is_hr_inside_hypersphere(const hyperrect& hr, const point& c, T r) const {
        array<T, D> p = hr.lf;
        for (size_t mask = 0; ;) {
            if (metric(p, c) > r + EPS) return false;
            if (++mask == (1 << D)) break;
            size_t j = 0;
            while (~mask >> j & 1) p[j] = hr.lf[j], ++j;
            p[j] = hr.rg[j];
        }
        return true;
    }

    //Works only for D = 2
    bool is_rect_intersect_circle(T x, T y, T r, hyperrect rect) const {
        static_assert(D == 2);
        T cdx = abs(x * 2 - rect.lf[0] - rect.rg[0]);
        T cdy = abs(y * 2 - rect.lf[1] - rect.rg[1]);
        T rw = rect.rg[0] - rect.lf[0];
        T rh = rect.rg[1] - rect.lf[1];
        if (cdx > rw + r * 2 + EPS || cdy > rh + r * 2 + EPS) return false;
        if (cdx <= rw + EPS || cdy <= rh + EPS) return true;
        return (cdx - rw) * (cdx - rw) + (cdy - rh) * (cdy - rh) <= 4 * r * r + EPS;
    }

    bool is_point_inside_hr(const hyperrect& hr, const point& p) const {
        for (size_t i = 0; i < D; ++i) {
            if (p[i] + EPS < hr.lf[i] || p[i] > hr.rg[i] + EPS) return false;
        }
        return true;
    }

    bool is_hr_inside_hr(const hyperrect& h, const hyperrect& hr) const {
        for (size_t i = 0; i < D; ++i) {
            if (h.lf[i] + EPS < hr.lf[i] || h.rg[i] > hr.rg[i] + EPS) return false;
        }
        return true;
    }

    bool is_hr_intersect_hr(const hyperrect& hr1, const hyperrect& hr2) const {
        for (size_t i = 0; i < D; ++i) {
            if (max(hr1.lf[i], hr2.lf[i]) > min(hr1.rg[i], hr2.rg[i]) + EPS) return false;
        }
        return true;
    }

    template<bool exclude_itself>
    void rec(const point& p, auto f, auto get_interesting_radius, auto ans, int cnt_exc_itself = 0) const {
        int cnt = 0;
        auto go = [&](auto&& go, size_t l, size_t r, size_t v) {
            if (l == r) {
                if constexpr(exclude_itself) {
                    if (m[l] == p && ++cnt <= cnt_exc_itself) {return;}
                }
                f(l);
                return;
            }
            size_t md = (l + r) >> 1;
            auto go_left = [&]() {go(go, l, md, v + 1);};
            auto go_right = [&]() {go(go, md + 1, r, v + 2 * (md - l + 1));};
            T df = p[vaxis[v]] - mid_axis[v];
            (df < 0 ? go_left() : go_right());
            if constexpr(IS_SQUARED_METRIC) {
                if (get_interesting_radius() >= df * df - EPS) (df < 0 ? go_right() : go_left());
            } else {
                if (get_interesting_radius() >= abs(df) - EPS) (df < 0 ? go_right() : go_left());
            }
        };
        go(go, 0, n - 1, 0);
    }

    //Euclidean distance squared. If need, change this function and parameter IS_SQUARED_METRIC.
    T metric(const point& p1, const point& p2) const {
        T res = 0;
        for (size_t i = 0; i < D; ++i) res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return res;
    }

public:
    KD_tree() = default;

    template<typename I>
    KD_tree(I first, I last): n(last - first), m(n), mid_axis(n * 2 - 1), bounding_box(n * 2 - 1), vaxis(n * 2 - 1) {
        if (!n) return;
        for (size_t i = 0; i < n; ++i) m[i] = *(first + i);
        auto build = [&](auto&& build, size_t l, size_t r, size_t v, size_t axis1 = 0) {
            if (l == r) {
                bounding_box[v] = {m[l], m[l]};
                return;
            }
            axis1 -= axis1 == D ? D : 0;
            size_t axis = axis1;
            if (MODE == LONGEST) {
                size_t best = 0;
                T longest = -1;
                for (size_t i = 0; i < D; ++i) {
                    T mn = m[l][i], mx = mn;
                    for (size_t j = l + 1; j <= r; ++j) {
                        mn = min(mn, m[j][i]);
                        mx = max(mx, m[j][i]);
                    }
                    if (mx - mn > longest) longest = mx - mn, best = i;
                }
                axis = best;
            }
            vaxis[v] = axis;
            size_t md = (l + r) / 2;
            nth_element(m.begin() + l, m.begin() + md, m.begin() + r + 1,
            [&](const point& a1, const point& a2) {return a1[axis] < a2[axis];});
            mid_axis[v] = m[md][axis];
            const size_t rn = v + 2 * (md - l + 1);
            build(build, l, md, v + 1, axis1 + 1);
            build(build, md + 1, r, rn, axis1 + 1);
            for (size_t i = 0; i < D; ++i) {
                bounding_box[v].lf[i] = min(bounding_box[v + 1].lf[i], bounding_box[rn].lf[i]);
                bounding_box[v].rg[i] = max(bounding_box[v + 1].rg[i], bounding_box[rn].rg[i]);
            }
        };
        build(build, 0, n - 1, 0);
        pnt = m[0];
        for (const point& p : m) {
            if (p != m[0]) {pnt = p; break;}
        }
    }

    KD_tree(vector<point>& points) {
        *this = KD_tree(points.begin(), points.end());
    }

    point closest_point(const point& p, int cnt_exclude_itself = 0) const {
        if (p == pnt && pnt == m[0]) return p;
        point ans = !cnt_exclude_itself ? m[0] : p != m[0] ? m[0] : pnt;
        auto f = [&](int l) {if (metric(p, m[l]) + EPS < metric(p, ans)) ans = m[l];};
        auto g = [&]() {return metric(p, ans);};
        if (!cnt_exclude_itself) rec<0>(p, f, g, ans);
        else rec<1>(p, f, g, ans, cnt_exclude_itself);
        return ans;
    }

    vector<point> k_closest_points(const point& p, const int k, int cnt_exclude_itself = 0) const {
        auto cmp = [&](const point& p1, const point& p2) {return metric(p, p1) < metric(p, p2);};
        priority_queue<point, vector<point>, decltype(cmp)> pq(cmp);
        auto g = [&]() {return pq.size() < k ? numeric_limits<T>::max() : metric(p, pq.top());};
        auto f = [&](int l) {pq.push(m[l]); if (pq.size() > k) pq.pop();};
        if (!cnt_exclude_itself) rec<0>(p, f, g, pq);
        else rec<1>(p, f, g, pq, cnt_exclude_itself);
        vector<point> ans(pq.size());
        for (size_t i = pq.size(); !pq.empty(); pq.pop()) ans[--i] = pq.top();
        return ans;
    }

    vector<point> all_closest_points(const point& p, int cnt_exclude_itself = 0) const {
        vector<point> ans;
        auto f = [&](int l) {
            if (ans.empty()) {ans = {m[l]}; return;}
            auto d1 = metric(p, m[l]), d2 = metric(p, ans[0]);
            if (d1 + EPS < d2) ans = {m[l]};
            else if (abs(d1 - d2) <= EPS) ans.push_back(m[l]);
        };
        auto g = [&]() {return ans.empty() ? numeric_limits<T>::max() : metric(p, ans[0]);};
        if (!cnt_exclude_itself) rec<0>(p, f, g, ans);
        else rec<1>(p, f, g, ans, cnt_exclude_itself);
        return ans;
    }

    //ans = |{p | metric(p, c) <= radius}|
    size_t count_points_inside_hypersphere(const point& c, T radius) const {
        T rr = IS_SQUARED_METRIC ? radius * radius : radius;
        size_t ans = 0;
        //Manually edit this constant to achieve better time
        const size_t MAGIC = 32;
        auto go = [&](auto&& go, size_t l, size_t r, size_t v) {
            if (r - l < MAGIC) {
                for (size_t i = l; i <= r; ++i) ans += metric(c, m[i]) <= rr + EPS;
                return;
            }
            if (is_hr_inside_hypersphere(bounding_box[v], c, rr)) {ans += r - l + 1; return;}
            if (!is_rect_intersect_circle(c[0], c[1], radius, bounding_box[v])) return;
            size_t md = (l + r) >> 1;
            auto go_left = [&]() {go(go, l, md, v + 1);};
            auto go_right = [&]() {go(go, md + 1, r, v + 2 * (md - l + 1));};
            T df = c[vaxis[v]] - mid_axis[v];
            (df < 0 ? go_left() : go_right());
            if (radius >= abs(df) - EPS) (df < 0 ? go_right() : go_left());
        };
        go(go, 0, n - 1, 0);
        return ans;
    }

    size_t count_points_inside_hyperrectangle(const hyperrect& hr) {
        for (size_t i = 0; i < D; ++i) assert(hr.lf[i] <= hr.rg[i] && "Bad hyperrect");
        size_t ans = 0;
        //Manually edit this constant to achieve better time
        const size_t MAGIC = 32;
        auto go = [&](auto&& go, size_t l, size_t r, size_t v) {
            if (r - l < MAGIC) {
                for (size_t i = l; i <= r; ++i) ans += is_point_inside_hr(hr, m[i]);
                return;
            }
            if (is_hr_inside_hr(bounding_box[v], hr)) {ans += r - l + 1; return;}
            if (!is_hr_intersect_hr(bounding_box[v], hr)) return;
            size_t md = (l + r) >> 1;
            if (hr.lf[vaxis[v]] <= mid_axis[v] + EPS) go(go, l, md, v + 1);
            if (mid_axis[v] <= hr.rg[vaxis[v]] + EPS) go(go, md + 1, r, v + 2 * (md - l + 1));
        };
        go(go, 0, n - 1, 0);
        return ans;
    }
};
