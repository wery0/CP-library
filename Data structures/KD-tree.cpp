//T - type of coordinates
//D - dimensions
/*
AXIS_MODE:
On each level of recursion points are splitting by
  CYCLIC: consecutive axes (0, 1, ... D - 1, 0, 1, ...)
  LONGEST: axis with the longest projection of all points
*/
//is_squared_metric - indicator of whether the metric returns the square of the distance or not
enum AXIS_MODE {CYCLIC, LONGEST};
template<typename T, const int D, const AXIS_MODE MODE = LONGEST, const bool is_squared_metric = true>
class KD_tree {
    static_assert(MODE == 0 || MODE == 1);
    static_assert(is_signed_v<T>);
    using point = array<T, D>;
    struct hyperrect {array<T, D> lf, rg;};

    //Change, if need.
    const T EPS = is_integral_v<T> ? 0 : 1e-9;

    size_t n;
    vector<point> m;
    vector<T> mid_axis;
    vector<size_t> vaxis;
    vector<hyperrect> bounding_box;

    bool is_hyperrect_inside_hypersphere(const hyperrect& hr, const point& c, T r) const {
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

    void rec(const point& p, auto f, auto get_interesting_radius, auto ans, int k = 1) const {
        auto go = [&](auto&& go, size_t l, size_t r, size_t v) {
            if (l == r) {f(l); return;}
            size_t md = (l + r) >> 1;
            auto go_left = [&]() {go(go, l, md, v + 1);};
            auto go_right = [&]() {go(go, md + 1, r, v + 2 * (md - l + 1));};
            T df = p[vaxis[v]] - mid_axis[v];
            (df < 0 ? go_left() : go_right());
            if constexpr(is_squared_metric) {
                if (get_interesting_radius() >= df * df - EPS) (df < 0 ? go_right() : go_left());
            } else {
                if (get_interesting_radius() >= abs(df) - EPS) (df < 0 ? go_right() : go_left());
            }
        };
        go(go, 0, n - 1, 0);
    }

    //Euclidean distance squared. Change, if need.
    T metric(const point& p1, const point& p2) const {
        T res = 0;
        for (size_t i = 0; i < D; ++i) res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return res;
    }

public:
    KD_tree() = default;

    template<typename I>
    KD_tree(I first, I last): n(last - first) {
        if (!n) return;
        m.resize(n);
        for (size_t i = 0; i < n; ++i) m[i] = *(first + i);
        mid_axis.resize(n * 2 - 1);
        bounding_box.resize(n * 2 - 1);
        vaxis.resize(n * 2 - 1);
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
    }

    KD_tree(vector<point>& points) {
        *this = KD_tree(points.begin(), points.end());
    }

    point closest_point(const point& p) const {
        point ans = m[0];
        auto f = [&](int l) {if (metric(p, m[l]) + EPS < metric(p, ans)) ans = m[l];};
        auto g = [&]() {return metric(p, ans);};
        rec(p, f, g, ans);
        return ans;
    }

    vector<point> k_closest_points(const int k, const point& p) const {
        auto cmp = [&](const point& p1, const point& p2) {return metric(p, p1) < metric(p, p2);};
        priority_queue<point, vector<point>, decltype(cmp)> pq(cmp);
        auto g = [&]() {return pq.size() < k ? numeric_limits<T>::max() : metric(p, pq.top());};
        auto f = [&](int l) {pq.push(m[l]); if (pq.size() > k) pq.pop();};
        rec(p, f, g, pq, k);
        vector<point> ans(pq.size());
        for (size_t i = pq.size(); !pq.empty(); pq.pop()) ans[--i] = pq.top();
        return ans;
    }

    vector<point> all_closest_points(const point& p) const {
        vector<point> ans;
        auto f = [&](int l) {
            if (ans.empty()) {ans = {m[l]}; return;}
            auto d1 = metric(p, m[l]), d2 = metric(p, ans[0]);
            if (d1 + EPS < d2) ans = {m[l]};
            else if (abs(d1 - d2) < EPS) ans.push_back(m[l]);
        };
        auto g = [&]() {return ans.empty() ? numeric_limits<T>::max() : metric(p, ans[0]);};
        rec(p, f, g, ans);
        return ans;
    }

    //ans = |{p | metric(p, c) <= radius}|
    size_t count_points_inside_hypersphere(const point& c, T radius) const {
        T rr = is_squared_metric ? radius * radius : radius;
        size_t ans = 0;
        //Manually edit these constants to achieve better time
        const size_t MAGIC1 = 32;
        const size_t MAGIC2 = 32;
        auto go = [&](auto&& go, size_t l, size_t r, size_t v) {
            if (r - l < MAGIC1) {
                for (size_t i = l; i <= r; ++i) ans += metric(c, m[i]) <= rr + EPS;
                return;
            }
            if (r - l > MAGIC2) {
                if (is_hyperrect_inside_hypersphere(bounding_box[v], c, rr)) {ans += r - l + 1; return;}
                if (!is_rect_intersect_circle(c[0], c[1], radius, bounding_box[v])) return;
            }
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
};